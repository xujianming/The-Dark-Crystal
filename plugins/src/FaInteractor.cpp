#include "FaInteractor.h"
#include "OBBoxRenderable.h"

#include <stdio.h>

using namespace std;

TiXmlElement* FaInteractorEditor::exportDotScene(TiXmlElement *pParent)
{
    Ogre::String type;
    PropertyOptionsVector *option = FaInteractorEditorFactory::GetInteractorTypes();
    
    for(auto iter = option->begin() ; iter != option->end() ; ++iter)
    {
        if(*Ogre::any_cast<int>(&(iter->mValue)) == mType->get())
        {
            type = iter->mKey;
        }
    }

    // interactor object 
    TiXmlElement *pInteractor = pParent->InsertEndChild(TiXmlElement("interactor"))->ToElement();
    pInteractor->SetAttribute("name", mName->get().c_str());
    pInteractor->SetAttribute("id", Ogre::StringConverter::toString(mObjectID->get()).c_str());
    pInteractor->SetAttribute("enabled", Ogre::StringConverter::toString(mIsEnabled->get()).c_str());
    pInteractor->SetAttribute("interactionObjectHandle", mInteractionObjectHandle->get().c_str());
    pInteractor->SetAttribute("range", Ogre::StringConverter::toString(mRange->get()).c_str());
    pInteractor->SetAttribute("offset", Ogre::StringConverter::toString(mOffset->get()).c_str());
    pInteractor->SetAttribute("interval", Ogre::StringConverter::toString(mInterval->get()).c_str());
    pInteractor->SetAttribute("type", type.c_str());
    
    // position
    TiXmlElement *pPosition = pInteractor->InsertEndChild(TiXmlElement("position"))->ToElement();
    pPosition->SetAttribute("x", Ogre::StringConverter::toString(mPosition->get().x).c_str());
    pPosition->SetAttribute("y", Ogre::StringConverter::toString(mPosition->get().y).c_str());
    pPosition->SetAttribute("z", Ogre::StringConverter::toString(mPosition->get().z).c_str());

    return pInteractor;
}


Ogre::AxisAlignedBox FaInteractorEditor::getAABB()
{
    if(mEntity)
    {
        return mEntity->getBoundingBox();
    } 
    else 
        return Ogre::AxisAlignedBox::BOX_NULL;
}

void FaInteractorEditor::setDerivedPosition(Ogre::Vector3 val)
{
    if(getParent())
    {
        Ogre::Quaternion qParent = getParent()->getDerivedOrientation().Inverse();
        Ogre::Vector3 vParent = getParent()->getDerivedPosition();
        Ogre::Vector3 newPos = (val - vParent);
        val = qParent * newPos;
    }
    mPosition->set(val);
}

Ogre::Vector3 FaInteractorEditor::getDerivedPosition()
{
    if(mHandle)
        return mHandle->_getDerivedPosition();
    else
        return getParent()->getDerivedPosition() + (getParent()->getDerivedOrientation() * mPosition->get());
}

void FaInteractorEditor::setSelectedImpl(bool bSelected)
{
    CBaseEditor::setSelectedImpl(bSelected);

    showBoundingBox(bSelected);
}

bool FaInteractorEditor::setHighlightedImpl(bool highlight )
{
    showBoundingBox(highlight);

    return true;
}

void FaInteractorEditor::showBoundingBox(bool bShow)
{
    if(!mBoxParentNode)
        createBoundingBox();

    if(mBBoxNode)
    {
        int matpos = 0;
        if(mHighlighted->get())
        {
            ++matpos;
            if(mSelected->get())
                ++matpos;
        }
        
        mOBBoxRenderable->setMaterial(mOBBMaterials[matpos]);
        mBBoxNode->setVisible(( bShow || mSelected->get() || mHighlighted->get() ));
    }
}

void FaInteractorEditor::createProperties(OgitorsPropertyValueMap &params)
{
    PROPERTY_PTR(mIsEnabled, "enabled", bool, true, 0, SETTER(bool, FaInteractorEditor, _setEnabled));
    PROPERTY_PTR(mPosition, "position",Ogre::Vector3,Ogre::Vector3::ZERO,0,SETTER(Ogre::Vector3, FaInteractorEditor, _setPosition));
    PROPERTY_PTR(mInteractionObjectHandle, "interactionObjectHandle", Ogre::String, "", 0, SETTER(Ogre::String, FaInteractorEditor, _setInteractionObjectHandle));
    PROPERTY_PTR(mRange, "range", Ogre::Real, 1.f, 0, SETTER(Ogre::Real, FaInteractorEditor, _setRange));
    PROPERTY_PTR(mOffset, "offset", Ogre::Real, 0.0, 0, SETTER(Ogre::Real, FaInteractorEditor, _setOffset));
    PROPERTY_PTR(mInterval, "interval", Ogre::Real, 0.0, 0, SETTER(Ogre::Real, FaInteractorEditor, _setInterval));
    PROPERTY_PTR(mType, "type", int, (int)RAYCASTING, 0, SETTER(int, FaInteractorEditor, _setType));
    mProperties.initValueMap(params);
}

bool FaInteractorEditor::_setPosition(OgitorsPropertyBase* property, const Ogre::Vector3& position)
{
    if(mHandle)
    {
      mHandle->setPosition(position);
    }

    _updatePaging();

    return true;
}

bool FaInteractorEditor::_setInteractionObjectHandle(OgitorsPropertyBase* property, const Ogre::String& handle)
{
    return true;
}

bool FaInteractorEditor::_setRange(OgitorsPropertyBase* property, const Ogre::Real& range)
{
    if(range >= 0.0)
        return true;
    else
        return false;
}

bool FaInteractorEditor::_setEnabled(OgitorsPropertyBase* property, const bool& is_enabled)
{
    return true;
}

bool FaInteractorEditor::_setInterval(OgitorsPropertyBase* property, const Ogre::Real& interval)
{
    if(interval >= 0.0)
        return true;
    else
        return false;
}

bool FaInteractorEditor::_setOffset(OgitorsPropertyBase* property, const Ogre::Real& offset)
{
    if(offset >= 0.0)
        return true;
    else
        return false;
}

bool FaInteractorEditor::_setType(OgitorsPropertyBase* property, const int& type)
{
    PropertyOptionsVector* options = FaInteractorEditorFactory::GetInteractorTypes();

    for(auto iter = options->begin() ; iter != options->end() ; ++iter)
    {
        if(*Ogre::any_cast<int>(&(iter->mValue)) == type)
            return true;
    }

    return false;
}

void FaInteractorEditor::setParentImpl(CBaseEditor *oldparent, CBaseEditor *newparent)
{
    if(!mHandle)
        return;

    if(oldparent) 
        oldparent->getNode()->removeChild(mHandle);
    
    if(newparent) 
        newparent->getNode()->addChild(mHandle);
}

bool FaInteractorEditor::load(bool async)
{
    cout << mName->get() << endl;
    if(mLoaded->get())
        return true;
    if(!getParent()->load())
        return false;

    //Create Physics Object
    mEntity = mOgitorsRoot->GetSceneManager()->createEntity(mName->get(), "FaInteractor.mesh");

    mHandle = getParent()->getNode()->createChildSceneNode(mName->get());
    mHandle->attachObject(mEntity);

    mHandle->setPosition(mPosition->get());

    mLoaded->set(true);

    return true;
}

bool FaInteractorEditor::unLoad()
{
    if(!mLoaded->get())
        return true;

    destroyBoundingBox();

    if(mHandle)
    {
        Ogre::Node *parent = mHandle->getParent();
        parent->removeChild(mHandle);
        mOgitorsRoot->GetSceneManager()->destroySceneNode(mHandle);
        mOgitorsRoot->GetSceneManager()->destroyEntity(mEntity);
        mHandle = 0;
        mEntity = 0;
    }
    
    mLoaded->set(false);
    return true;
}

FaInteractorEditor::FaInteractorEditor(CBaseEditorFactory *factory)
    : CBaseEditor(factory)
{
    mHandle = 0;
    mPosition = 0;
    mInteractionObjectHandle = 0;
    mRange = 0;
    mOffset = 0;
    mInterval = 0;
    mIsEnabled = 0;
    mType = (int)RAYCASTING;
    mUsesGizmos = true;
}

FaInteractorEditor::~FaInteractorEditor()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////
PropertyOptionsVector FaInteractorEditorFactory::mInteractorTypes;

FaInteractorEditorFactory::FaInteractorEditorFactory(OgitorsView *view) : CBaseEditorFactory(view)
{
    mTypeName = "Interactor Object";
    mEditorType = ETYPE_NODE;
    mAddToObjectList = true;
    mRequirePlacement = true;
    mIcon = "Icons/interactor.svg";//----------------
    mCapabilities = CAN_MOVE|CAN_DELETE|CAN_CLONE|CAN_FOCUS|CAN_DRAG|CAN_ACCEPTCOPY;

    mInteractorTypes.clear();
    mInteractorTypes.push_back(PropertyOption("RAYCASTING", Ogre::Any((int)FaInteractorEditor::RAYCASTING)));
    mInteractorTypes.push_back(PropertyOption("COLLISION", Ogre::Any((int)FaInteractorEditor::COLLISION)));

    AddPropertyDefinition("enabled", "Enabled", "If this component is enabled.", PROP_BOOL);
    AddPropertyDefinition("position","Position","The position of the object.", PROP_VECTOR3);
    AddPropertyDefinition("interactionObjectHandle", "Handle", "The handle of the interaction object.", PROP_STRING);
    AddPropertyDefinition("range","Range","The range of the interactor.", PROP_REAL);
    AddPropertyDefinition("offset","Offset","The offset of the interactor.", PROP_REAL);
    AddPropertyDefinition("interval","Interval Time","The interval time between two interactions.", PROP_REAL);
    AddPropertyDefinition("type","Interactor Type","The type of the interactor. Raycasting or collision.", PROP_INT)->setOptions(&mInteractorTypes);
}

CBaseEditorFactory *FaInteractorEditorFactory::duplicate(OgitorsView *view)
{
    CBaseEditorFactory *ret = OGRE_NEW FaInteractorEditorFactory(view);
    ret->mTypeID = mTypeID;

    return ret;
}

CBaseEditor *FaInteractorEditorFactory::CreateObject(CBaseEditor **parent, OgitorsPropertyValueMap &params)
{
    FaInteractorEditor *object = OGRE_NEW FaInteractorEditor(this);
    OgitorsPropertyValueMap::iterator ni;

    if ((ni = params.find("init")) != params.end())
    {
        OgitorsPropertyValue value = EMPTY_PROPERTY_VALUE;
        value.val = Ogre::Any(CreateUniqueID("Interactor"));
        params["name"] = value;
        params.erase(ni);
    }

    object->createProperties(params);
    object->mParentEditor->init(*parent);
    object->registerForPostSceneUpdates();

    mInstanceCount++;
    return object;
}

Ogre::String FaInteractorEditorFactory::GetPlaceHolderName()
{
    return "FaInteractor.mesh";
}

PropertyOptionsVector* FaInteractorEditorFactory::GetInteractorTypes()
{
    return &mInteractorTypes;
}
/////////////////////////////////////////////////////////////////////////
FaInteractorEditorFactory *faInteractorFac = 0;

bool dllStartPlugin(void *identifier, Ogre::String& name)
{
    name = "Interactor Object";
    faInteractorFac = OGRE_NEW FaInteractorEditorFactory();
    OgitorsRoot::getSingletonPtr()->RegisterEditorFactory(identifier, faInteractorFac);
    return true;
}

bool dllStopPlugin(void)
{
    OGRE_DELETE faInteractorFac;
    faInteractorFac = 0;
    return true;
}
