
#include "ezGfxPlusRefCounted.h"
#include <memory>

using namespace ezGfxPlus;


class RefCounted::Impl
{
public:
	Impl() {}
	Impl(int refCount) : m_refCount(new int(refCount)) {}
	Impl(const Impl& other) : m_refCount(other.m_refCount) {}

	std::shared_ptr<int> m_refCount;
};

RefCounted::RefCounted()
: impl( new Impl(1) )
{

}

RefCounted::RefCounted(const RefCounted& other)
: impl( new Impl(*other.impl) )
{
	
}

RefCounted::~RefCounted()
{
	delete impl;
}

int RefCounted::refCount() const
{
	return *impl->m_refCount;
}

void RefCounted::addRef()
{
	++(*impl->m_refCount);
}

void RefCounted::release()
{
	--(*impl->m_refCount);
}