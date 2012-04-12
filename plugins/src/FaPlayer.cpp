#include "FaPlayer.h"
#include "OBBoxRenderable.h"

#include <stdio.h>

using namespace std;

TiXmlElement* PlayerEditor::exportDotScene(TiXmlElement *pParent)
{
    // player object 
    TiXmlElement *pPlayer = pParent->InsertEndChild(TiXmlElement("physics"))->ToElement();
    pPlayer->SetAttribute("name", mName->get().c_str());
    pPlayer->SetAttribute("id", Ogre::StringConverter::toString(mObjectID->get()).c_str());
    pPlayer->SetAttribute("enabled", Ogre::StringConverter::toString(mIsEnabled->get()).c_str());

    // position
    TiXmlElement *pPosition = pPlayer->InsertEndChild(TiXmlElement("position"))->ToElement();
    pPosition->SetAttribute("x", Ogre::StringConverter::toString(mPosition->get().x).c_str());
    pPosition->SetAttribute("y", Ogre::StringConverter::toString(mPosition->get().y).c_str());
    pPosition->SetAttribute("z", Ogre::StringConverter::toString(mPosition->get().z).c_str());

    return pPlayer;
}


Ogre::AxisAlignedBox PlayerEditor::getAABB()
{
    if(mEntity)
    {
        return mEntity->getBoundingBox();
    } 
    else 
        return Ogre::AxisAlignedBox::BOX_NULL;
}

void PlayerEditor::setDerivedPosition(Ogre::Vector3 val)
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

Ogre::Vector3 PlayerEditor::getDerivedPosition()
{
    if(mHandle)
        return mHandle->_getDerivedPosition();
    else
        return getParent()->getDerivedPosition() + (getParent()->getDerivedOrientation() * mPosition->get());
}

void PlayerEditor::setSelectedImpl(bool bSelected)
{
    CBaseEditor::setSelectedImpl(bSelected);

    showBoundingBox(bSelected);
}

bool PlayerEditor::setHighlightedImpl(bool highlight )
{
    showBoundingBox(highlight);

    return true;
}

void PlayerEditor::showBoundingBox(bool bShow)
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

void PlayerEditor::createProperties(OgitorsPropertyValueMap &params)
{
    PROPERTY_PTR(mIsEnabled, "enabled", bool, false, 0, SETTER(bool, PlayerEditor, _setEnabled));
    PROPERTY_PTR(mPosition, "position", Ogre::Vector3, Ogre::Vector3::ZERO, 0, SETTER(Ogre::Vector3, PlayerEditor, _setPosition));
    mProperties.initValueMap(params);
}

bool PlayerEditor::_setPosition(OgitorsPropertyBase* property, const Ogre::Vector3& position)
{
    if(mHandle)
    {
      mHandle->setPosition(position);
    }

    _updatePaging();

    return true;
}

bool PlayerEditor::_setEnabled(OgitorsPropertyBase* property, const bool& is_enabled)
{
    if(this->getParent()->getNode())
        return true;
    else
        return false;
}

void PlayerEditor::setParentImpl(CBaseEditor *oldparent, CBaseEditor *newparent)
{
    if(!mHandle)
        return;

    if(oldparent) 
        oldparent->getNode()->removeChild(mHandle);
    
    if(newparent) 
        newparent->getNode()->addChild(mHandle);
}

bool PlayerEditor::load(bool async)
{
    cout << mName->get() << endl;
    if(mLoaded->get())
        return true;
    if(!getParent()->load())
        return false;

    //Create Physics Object
    mEntity = mOgitorsRoot->GetSceneManager()->createEntity(mName->get(), "Fysics.mesh");

    mHandle = getParent()->getNode()->createChildSceneNode(mName->get());
    mHandle->attachObject(mEntity);

    mHandle->setPosition(mPosition->get());

    mLoaded->set(true);

    return true;
}

bool PlayerEditor::unLoad()
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

PlayerEditor::PlayerEditor(CBaseEditorFactory *factory)
    : CBaseEditor(factory)
{
    mHandle = 0;
    mPosition = 0;
    mIsEnabled = 0;
    mUsesGizmos = true;
}

PlayerEditor::~PlayerEditor()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////
PlayerEditorFactory::PlayerEditorFactory(OgitorsView *view) : CBaseEditorFactory(view)
{
    mTypeName = "Controller Object";
    mEditorType = ETYPE_NODE;
    mAddToObjectList = true;
    mRequirePlacement = true;
    mIcon = "Icons/Controller.svg";//----------------
    mCapabilities = CAN_MOVE|CAN_DELETE|CAN_CLONE|CAN_FOCUS|CAN_DRAG|CAN_ACCEPTCOPY;

    AddPropertyDefinition("enabled", "Enabled", "If this component is enabled.", PROP_BOOL);
    AddPropertyDefinition("position", "Position", "The position of the object.", PROP_VECTOR3);
}

CBaseEditorFactory *PlayerEditorFactory::duplicate(OgitorsView *view)
{
    CBaseEditorFactory *ret = OGRE_NEW PlayerEditorFactory(view);
    ret->mTypeID = mTypeID;

    return ret;
}

CBaseEditor *PlayerEditorFactory::CreateObject(CBaseEditor **parent, OgitorsPropertyValueMap &params)
{
    PlayerEditor *object = OGRE_NEW PlayerEditor(this);
    OgitorsPropertyValueMap::iterator ni;

    if ((ni = params.find("init")) != params.end())
    {
        OgitorsPropertyValue value = EMPTY_PROPERTY_VALUE;
        value.val = Ogre::Any(CreateUniqueID("Controller"));
        params["name"] = value;
        params.erase(ni);
    }

    object->createProperties(params);
    object->mParentEditor->init(*parent);
    object->registerForPostSceneUpdates();

    mInstanceCount++;
    return object;
}

Ogre::String PlayerEditorFactory::GetPlaceHolderName()
{
    return "Controller.mesh";
}

/////////////////////////////////////////////////////////////////////////
PlayerEditorFactory *playerFac = 0;

bool dllStartPlugin(void *identifier, Ogre::String& name)
{
    name = "Controller Object";
    playerFac = OGRE_NEW PlayerEditorFactory();
    OgitorsRoot::getSingletonPtr()->RegisterEditorFactory(identifier, playerFac);
    return true;
}

bool dllStopPlugin(void)
{
    OGRE_DELETE playerFac;
    playerFac = 0;
    return true;
}
