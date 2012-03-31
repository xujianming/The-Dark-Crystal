#include "FaSound.h"
#include "OBBoxRenderable.h"

#include <stdio.h>

using namespace std;

TiXmlElement* FaSoundEditor::exportDotScene(TiXmlElement *pParent)
{
    // sound
    TiXmlElement *pSound = pParent->InsertEndChild(TiXmlElement("sound"))->ToElement();
    pSound->SetAttribute("name", mName->get().c_str());
    pSound->SetAttribute("id", Ogre::StringConverter::toString(mObjectID->get()).c_str());
    pSound->SetAttribute("enabled", Ogre::StringConverter::toString(mIsEnabled->get()).c_str());
    pSound->SetAttribute("soundFile", mSoundFile->get().c_str());
    pSound->SetAttribute("volume", Ogre::StringConverter::toString(mVol->get()).c_str());
    pSound->SetAttribute("loop", Ogre::StringConverter::toString(mIsLoop->get()).c_str());
    // sound position
    TiXmlElement *pSoundPosition = pSound->InsertEndChild(TiXmlElement("position"))->ToElement();
    pSoundPosition->SetAttribute("x", Ogre::StringConverter::toString(mPosition->get().x).c_str());
    pSoundPosition->SetAttribute("y", Ogre::StringConverter::toString(mPosition->get().y).c_str());
    pSoundPosition->SetAttribute("z", Ogre::StringConverter::toString(mPosition->get().z).c_str());

   return pSound;
}


Ogre::AxisAlignedBox FaSoundEditor::getAABB()
{
    if(mEntity)
    {
        return mEntity->getBoundingBox();
    } 
    else 
        return Ogre::AxisAlignedBox::BOX_NULL;
}

void FaSoundEditor::setDerivedPosition(Ogre::Vector3 val)
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

Ogre::Vector3 FaSoundEditor::getDerivedPosition()
{
    if(mHandle)
        return mHandle->_getDerivedPosition();
    else
        return getParent()->getDerivedPosition() + (getParent()->getDerivedOrientation() * mPosition->get());
}

void FaSoundEditor::setSelectedImpl(bool bSelected)
{
    CBaseEditor::setSelectedImpl(bSelected);

    showBoundingBox(bSelected);
}

bool FaSoundEditor::setHighlightedImpl(bool highlight )
{
    showBoundingBox(highlight);

    return true;
}

void FaSoundEditor::showBoundingBox(bool bShow)
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

void FaSoundEditor::createProperties(OgitorsPropertyValueMap &params)
{
    PROPERTY_PTR(mIsEnabled, "enabled", bool, true, 0, SETTER(bool, FaSoundEditor, _setEnabled));
    PROPERTY_PTR(mPosition , "position" , Ogre::Vector3, Ogre::Vector3::ZERO, 0, SETTER(Ogre::Vector3, FaSoundEditor, _setPosition));
    PROPERTY_PTR(mSoundFile, "SoundFile", Ogre::String , ""                 , 0, SETTER(Ogre::String, FaSoundEditor, _setSoundFile));
    PROPERTY_PTR(mVol      , "Volume"   , Ogre::Real   , 80.0f              , 0, SETTER(Ogre::Real, FaSoundEditor, _setVolume));
    PROPERTY_PTR(mIsLoop, "loop", bool, false, 0, SETTER(bool, FaSoundEditor, _setLoop));
    mProperties.initValueMap(params);
}

bool FaSoundEditor::_setSoundFile(OgitorsPropertyBase* property, const Ogre::String& soundfile)
{
  return true;
}

bool FaSoundEditor::_setPosition(OgitorsPropertyBase* property, const Ogre::Vector3& position)
{
    if(mHandle)
    {
      mHandle->setPosition(position);
    }

    _updatePaging();

    return true;
}

bool FaSoundEditor::_setVolume(OgitorsPropertyBase* property, const Ogre::Real& vol)
{
    if( (0 > vol)||(100 < vol) )
        return false;
    return true;
}

bool FaSoundEditor::_setEnabled(OgitorsPropertyBase* property, const bool& is_enabled)
{
    return true;
}

bool FaSoundEditor::_setLoop(OgitorsPropertyBase* property, const bool& is_loop)
{
    return true;
}

void FaSoundEditor::setParentImpl(CBaseEditor *oldparent, CBaseEditor *newparent)
{
    if(!mHandle)
        return;

    if(oldparent) 
        oldparent->getNode()->removeChild(mHandle);
    
    if(newparent) 
        newparent->getNode()->addChild(mHandle);
}

bool FaSoundEditor::load(bool async)
{
cout<<mName->get()<<endl;
    if(mLoaded->get())
        return true;
    if(!getParent()->load())
        return false;

    //Create Sound Object
    mEntity = mOgitorsRoot->GetSceneManager()->createEntity(mName->get(), "FaSound.mesh");

    mHandle = getParent()->getNode()->createChildSceneNode(mName->get());
    mHandle->attachObject(mEntity);

    mHandle->setPosition(mPosition->get());

    mLoaded->set(true);

    return true;
}

bool FaSoundEditor::unLoad()
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

FaSoundEditor::FaSoundEditor(CBaseEditorFactory *factory)
    : CBaseEditor(factory)
{
    mHandle = 0;
    mPosition = 0;
    mUsesGizmos = true;
    mVol = 0;
    mIsEnabled = 0;
    mIsLoop = 0;
}

FaSoundEditor::~FaSoundEditor()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////

FaSoundEditorFactory::FaSoundEditorFactory(OgitorsView *view) : CBaseEditorFactory(view)
{
    mTypeName = "Sound Object";
    mEditorType = ETYPE_NODE;
    mAddToObjectList = true;
    mRequirePlacement = true;
    mIcon = "Icons/FaSound.svg";//----------------
    mCapabilities = CAN_MOVE|CAN_DELETE|CAN_CLONE|CAN_FOCUS|CAN_DRAG|CAN_ACCEPTCOPY;

    AddPropertyDefinition("enabled", "Enabled", "If this component is enabled.", PROP_BOOL);
    AddPropertyDefinition("SoundFile","Sound File","",PROP_STRING);
    AddPropertyDefinition("position","Position","The position of the object.",PROP_VECTOR3);
    AddPropertyDefinition("Volume", "Volume", "The sound object's volume.0-100.",PROP_REAL);
    AddPropertyDefinition("loop", "Loop", "If the sound is loop.", PROP_BOOL);
}

CBaseEditorFactory *FaSoundEditorFactory::duplicate(OgitorsView *view)
{
    CBaseEditorFactory *ret = OGRE_NEW FaSoundEditorFactory(view);
    ret->mTypeID = mTypeID;

    return ret;
}

CBaseEditor *FaSoundEditorFactory::CreateObject(CBaseEditor **parent, OgitorsPropertyValueMap &params)
{
    FaSoundEditor *object = OGRE_NEW FaSoundEditor(this);
    OgitorsPropertyValueMap::iterator ni;

    if ((ni = params.find("init")) != params.end())
    {
        OgitorsPropertyValue value = EMPTY_PROPERTY_VALUE;
        value.val = Ogre::Any(CreateUniqueID("Sound"));
        params["name"] = value;
        params.erase(ni);
    }

    object->createProperties(params);
    object->mParentEditor->init(*parent);
    object->registerForPostSceneUpdates();

    mInstanceCount++;
    return object;
}

Ogre::String FaSoundEditorFactory::GetPlaceHolderName()
{
    return "FaSound.mesh";
}
/////////////////////////////////////////////////////////////////////////
FaSoundEditorFactory *fasoundFac = 0;

bool dllStartPlugin(void *identifier, Ogre::String& name)
{
    name = "FaSound Object";
    fasoundFac = OGRE_NEW FaSoundEditorFactory();
    OgitorsRoot::getSingletonPtr()->RegisterEditorFactory(identifier, fasoundFac);
    return true;
}

bool dllStopPlugin(void)
{
    OGRE_DELETE fasoundFac;
    fasoundFac = 0;
    return true;
}
