#ifndef SCROLLLIST_MYCONTAINER_HPP
#define SCROLLLIST_MYCONTAINER_HPP

#include <gui_generated/containers/ScrollList_myContainerBase.hpp>

class ScrollList_myContainer : public ScrollList_myContainerBase
{
public:
    ScrollList_myContainer();
    virtual ~ScrollList_myContainer() {}

    virtual void initialize();
protected:
};

#endif // SCROLLLIST_MYCONTAINER_HPP
