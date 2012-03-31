#include "FaMusic.h"
#include "OBBoxRenderable.h"

#include <stdio.h>

using namespace std;

TiXmlElement* FaMusicEditor::exportDotScene(TiXmlElement *pParent)
{
    // interactor object 
    TiXmlElement *pMusic = pParent->InsertEndChild(TiXmlElement("music"))->ToElement();
    pMusic->SetAttribute("name", mName->get().c_str());
    pMusic->SetAttribute("id", Ogre::StringConverter::toString(mObjectID->get()).c_str());
    pMusic->SetAttribute("enabled", Ogre::StringConverter::toString(mIsEnabled->get()).c_str());
    pMusic->SetAttribute("musicPath", mMusicHandle->get().c_str());
    pMusic->SetAttribute("volume", Ogre::StringConverter::toString(mVol->get()).c_str());
    pMusic->SetAttribute("loop", Ogre::StringConverter::toString(mIsLoop->get()).c_str());
    
    // position
    TiXmlElement *pPosition = pMusic->InsertEndChild(TiXmlElement("position"))->ToElement();
    pPosition->SetAttribute("x", Ogre::StringConverter::toString(mPosition->get().x).c_str());
    pPosition->SetAttribute("y", Ogre::StringConverter::toString(mPosition->get().y).c_str());
    pPosition->SetAttribute("z", Ogre::StringConverter::toString(mPosition->get().z).c_str());

    return pMusic;
}


Ogre::AxisAlignedBox FaMusicEditor::getAABB()
{
    if(mEntity)
    {
        return mEntity->getBoundingBox();
    } 
    else 
        return Ogre::AxisAlignedBox::BOX_NULL;
}

void FaMusicEditor::setDerivedPosition(Ogre::Vector3 val)
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

Ogre::Vector3 FaMusicEditor::getDerivedPosition()
{
    if(mHandle)
        return mHandle->_getDerivedPosition();
    else
        return getParent()->getDerivedPosition() + (getParent()->getDerivedOrientation() * mPosition->get());
}

void FaMusicEditor::setSelectedImpl(bool bSelected)
{
    CBaseEditor::setSelectedImpl(bSelected);

    showBoundingBox(bSelected);
}

bool FaMusicEditor::setHighlightedImpl(bool highlight )
{
    showBoundingBox(highlight);

    return true;
}

void FaMusicEditor::showBoundingBox(bool bShow)
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

void FaMusicEditor::createProperties(OgitorsPropertyValueMap &params)
{
    PROPERTY_PTR(mIsEnabled, "enabled", bool, true, 0, SETTER(bool, FaMusicEditor, _setEnabled));
    PROPERTY_PTR(mPosition, "position", Ogre::Vector3,Ogre::Vector3::ZERO,0,SETTER(Ogre::Vector3, FaMusicEditor, _setPosition));
    PROPERTY_PTR(mVol, "volume", Ogre::Real, 0.0f, 0, SETTER(Ogre::Real, FaMusicEditor, _setVolume));
    PROPERTY_PTR(mMusicHandle, "music_path", Ogre::String, "", 0, SETTER(Ogre::String, FaMusicEditor, _setMusicHandle));
    PROPERTY_PTR(mIsLoop, "loop", bool, false, 0, SETTER(bool, FaMusicEditor, _setLoop));
    mProperties.initValueMap(params);
}

bool FaMusicEditor::_setPosition(OgitorsPropertyBase* property, const Ogre::Vector3& position)
{
    if(mHandle)
    {
      mHandle->setPosition(position);
    }

    _updatePaging();

    return true;
}

bool FaMusicEditor::_setMusicHandle(OgitorsPropertyBase* property, const Ogre::String& music_handle)
{
    return true;
}

bool FaMusicEditor::_setVolume(OgitorsPropertyBase* property, const Ogre::Real& vol)
{
    if(vol >= 0.0f && vol <= 100.0f)
        return true;
    else
        return false;
}

bool FaMusicEditor::_setEnabled(OgitorsPropertyBase* property, const bool& is_enabled)
{
    return true;
}

bool FaMusicEditor::_setLoop(OgitorsPropertyBase* property, const bool& is_loop)
{
    return true;
}

void FaMusicEditor::setParentImpl(CBaseEditor *oldparent, CBaseEditor *newparent)
{
    if(!mHandle)
        return;

    if(oldparent) 
        oldparent->getNode()->removeChild(mHandle);
    
    if(newparent) 
        newparent->getNode()->addChild(mHandle);
}

bool FaMusicEditor::load(bool async)
{
    cout << mName->get() << endl;
    if(mLoaded->get())
        return true;
    if(!getParent()->load())
        return false;

    //Create Physics Object
    mEntity = mOgitorsRoot->GetSceneManager()->createEntity(mName->get(), "FaMusic.mesh");

    mHandle = getParent()->getNode()->createChildSceneNode(mName->get());
    mHandle->attachObject(mEntity);

    mHandle->setPosition(mPosition->get());

    mLoaded->set(true);

    return true;
}

bool FaMusicEditor::unLoad()
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

FaMusicEditor::FaMusicEditor(CBaseEditorFactory *factory)
    : CBaseEditor(factory)
{
    mHandle = 0;
    mPosition = 0;
    mMusicHandle = 0;
    mVol = 0;
    mIsEnabled = 0;
    mIsLoop = 0;
    mUsesGizmos = true;
}

FaMusicEditor::~FaMusicEditor()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////
FaMusicEditorFactory::FaMusicEditorFactory(OgitorsView *view) : CBaseEditorFactory(view)
{
    mTypeName = "Music Object";
    mEditorType = ETYPE_NODE;
    mAddToObjectList = true;
    mRequirePlacement = true;
    mIcon = "Icons/music.svg";//----------------
    mCapabilities = CAN_MOVE|CAN_DELETE|CAN_CLONE|CAN_FOCUS|CAN_DRAG|CAN_ACCEPTCOPY;

    AddPropertyDefinition("enabled","Enabled","If this component is enabled.", PROP_BOOL);
    AddPropertyDefinition("position","Position","The position of the object.", PROP_VECTOR3);
    AddPropertyDefinition("music_path", "Music_Path", "The path of the music file.", PROP_STRING);
    AddPropertyDefinition("volume","Volume","The volume of the music.", PROP_REAL);
    AddPropertyDefinition("loop","Loop","If looping of this music is enabled.", PROP_BOOL);
}

CBaseEditorFactory *FaMusicEditorFactory::duplicate(OgitorsView *view)
{
    CBaseEditorFactory *ret = OGRE_NEW FaMusicEditorFactory(view);
    ret->mTypeID = mTypeID;

    return ret;
}

CBaseEditor *FaMusicEditorFactory::CreateObject(CBaseEditor **parent, OgitorsPropertyValueMap &params)
{
    FaMusicEditor *object = OGRE_NEW FaMusicEditor(this);
    OgitorsPropertyValueMap::iterator ni;

    if ((ni = params.find("init")) != params.end())
    {
        OgitorsPropertyValue value = EMPTY_PROPERTY_VALUE;
        value.val = Ogre::Any(CreateUniqueID("Music"));
        params["name"] = value;
        params.erase(ni);
    }

    object->createProperties(params);
    object->mParentEditor->init(*parent);
    object->registerForPostSceneUpdates();

    mInstanceCount++;
    return object;
}

Ogre::String FaMusicEditorFactory::GetPlaceHolderName()
{
    return "FaMusic.mesh";
}
/////////////////////////////////////////////////////////////////////////
FaMusicEditorFactory *faMusicFac = 0;

bool dllStartPlugin(void *identifier, Ogre::String& name)
{
    name = "Music Object";
    faMusicFac = OGRE_NEW FaMusicEditorFactory();
    OgitorsRoot::getSingletonPtr()->RegisterEditorFactory(identifier, faMusicFac);
    return true;
}

bool dllStopPlugin(void)
{
    OGRE_DELETE faMusicFac;
    faMusicFac = 0;
    return true;
}
