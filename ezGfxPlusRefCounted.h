
#ifndef EZGFXPLUS_REFCOUNTED_H
#define EZGFXPLUS_REFCOUNTED_H

#include "ezGfxPlus.h"

namespace ezGfxPlus
{
	class EZGFXPLUS_API RefCounted
	{
	public:
		RefCounted();
		RefCounted(const RefCounted& other);
		~RefCounted();

		int refCount() const;

	protected:
		void addRef();
		void release();

	private:
		class Impl;
		Impl *impl;
	};
}

#endif // EZGFXPLUS_REFCOUNTED_H