
#include <Windows.h>
#include <math.h>
#include <iostream>

using namespace std;

#define HALF_NOTE 1.059463094359 // HALF_NOTE ^ 12 = 2
#define PI 3.14159265358979


class SoundEffect
{
public:
	SoundEffect()
	{
		m_data = NULL;
	}
	SoundEffect(const int noteInfo[], const int arraySize)
	{
		// Initialize the sound format we will request from sound card
		m_waveFormat.wFormatTag = WAVE_FORMAT_PCM;     // Uncompressed sound format
		m_waveFormat.nChannels = 1;                    // 1 = Mono, 2 = Stereo
		m_waveFormat.wBitsPerSample = 8;               // Bits per sample per channel
		m_waveFormat.nSamplesPerSec = 11025;           // Sample Per Second
		m_waveFormat.nBlockAlign = m_waveFormat.nChannels * m_waveFormat.wBitsPerSample / 8;
		m_waveFormat.nAvgBytesPerSec = m_waveFormat.nSamplesPerSec * m_waveFormat.nBlockAlign;
		m_waveFormat.cbSize = 0;

		int dataLength = 0, moment = (m_waveFormat.nSamplesPerSec / 75);
		double period = 2.0 * PI / (double)m_waveFormat.nSamplesPerSec;

		// Calculate how long we need the sound buffer to be
		for (int i = 1; i < arraySize; i += 2)
			dataLength += (noteInfo[i] != 0) ? noteInfo[i] * moment : moment;

		// Allocate the array
		m_data = new char[m_bufferSize = dataLength];

		int placeInData = 0;

		// Make the sound buffer
		for (int i = 0; i < arraySize; i += 2)
		{
			int relativePlaceInData = placeInData;

			while ((relativePlaceInData - placeInData) < ((noteInfo[i + 1] != 0) ? noteInfo[i + 1] * moment : moment))
			{
				// Generate the sound wave (as a sinusoid)
				// - x will have a range of -1 to +1
				double x = sin((relativePlaceInData - placeInData) * 55 * pow(HALF_NOTE, noteInfo[i]) * period);

				// Scale x to a range of 0-255 (signed char) for 8 bit sound reproduction
				m_data[relativePlaceInData] = (char)(127 * x + 128);

				relativePlaceInData++;
			}

			placeInData = relativePlaceInData;
		}
	}
	SoundEffect(SoundEffect& otherInstance)
	{
		m_bufferSize = otherInstance.m_bufferSize;
		m_waveFormat = otherInstance.m_waveFormat;

		if (m_bufferSize > 0)
		{
			m_data = new char[m_bufferSize];

			for (int i = 0; i < otherInstance.m_bufferSize; i++)
				m_data[i] = otherInstance.m_data[i];
		}
	}
	~SoundEffect()
	{
		if (m_bufferSize > 0)
			delete[] m_data;
	}

	SoundEffect& operator=(SoundEffect& otherInstance)
	{
		if (m_bufferSize > 0)
			delete[] m_data;

		m_bufferSize = otherInstance.m_bufferSize;
		m_waveFormat = otherInstance.m_waveFormat;

		if (m_bufferSize > 0)
		{
			m_data = new char[m_bufferSize];

			for (int i = 0; i < otherInstance.m_bufferSize; i++)
				m_data[i] = otherInstance.m_data[i];
		}

		return *this;
	}

	void Play()
	{
		// Create our "Sound is Done" event
		m_done = CreateEvent(0, FALSE, FALSE, 0);

		// Open the audio device
		if (waveOutOpen(&m_waveOut, 0, &m_waveFormat, (DWORD)m_done, 0, CALLBACK_EVENT) != MMSYSERR_NOERROR)
		{
			cout << "Sound card cannot be opened." << endl;
			return;
		}

		// Create the wave header for our sound buffer
		m_waveHeader.lpData = m_data;
		m_waveHeader.dwBufferLength = m_bufferSize;
		m_waveHeader.dwFlags = 0;
		m_waveHeader.dwLoops = 0;

		// Prepare the header for playback on sound card
		if (waveOutPrepareHeader(m_waveOut, &m_waveHeader, sizeof(m_waveHeader)) != MMSYSERR_NOERROR)
		{
			cout << "Error preparing Header!" << endl;
			return;
		}

		// Play the sound!
		ResetEvent(m_done); // Reset our Event so it is non-signaled, it will be signaled again with buffer finished

		if (waveOutWrite(m_waveOut, &m_waveHeader, sizeof(m_waveHeader)) != MMSYSERR_NOERROR)
		{
			cout << "Error writing to sound card!" << endl;
			return;
		}

		// Wait until sound finishes playing
		if (WaitForSingleObject(m_done, INFINITE) != WAIT_OBJECT_0)
		{
			cout << "Error waiting for sound to finish" << endl;
			return;
		}

		// Unprepare our wav header
		if (waveOutUnprepareHeader(m_waveOut, &m_waveHeader, sizeof(m_waveHeader)) != MMSYSERR_NOERROR)
		{
			cout << "Error unpreparing header!" << endl;
			return;
		}

		// Close the wav device
		if (waveOutClose(m_waveOut) != MMSYSERR_NOERROR)
		{
			cout << "Sound card cannot be closed!" << endl;
			return;
		}

		// Release our event handle
		CloseHandle(m_done);
	}

private:
	HWAVEOUT m_waveOut; // Handle to sound card output
	WAVEFORMATEX m_waveFormat; // The sound format
	WAVEHDR m_waveHeader; // WAVE header for our sound data
	HANDLE m_done; // Event Handle that tells us the sound has finished being played.
	// This is a very efficient way to put the program to sleep
	// while the sound card is processing the sound buffer

	char* m_data; // Sound data buffer
	int m_bufferSize; // Size of sound data buffer
};