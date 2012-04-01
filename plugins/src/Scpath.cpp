#include "Scpath.h"
#include "OBBoxRenderable.h"

#include <stdio.h>

using namespace std;


TiXmlElement* ScpathEditor::exportDotScene(TiXmlElement *pParent) {
    // scpath object
    TiXmlElement *pScpath = pParent->InsertEndChild(TiXmlElement("scpath"))->ToElement();
    pScpath->SetAttribute("name", mName->get().c_str());
    pScpath->SetAttribute("id", Ogre::StringConverter::toString(mObjectID->get()).c_str());
    pScpath->SetAttribute("enabled", Ogre::StringConverter::toString(mIsEnabled->get()).c_str());
	pScpath->SetAttribute("path", mPath->get().c_str());
    pScpath->SetAttribute("updateCallEnabled", Ogre::StringConverter::toString(mIsUpdateCallEnabled->get()).c_str());
    // object position
    TiXmlElement *pScpathPosition = pScpath->InsertEndChild(TiXmlElement("position"))->ToElement();
    pScpathPosition->SetAttribute("x", Ogre::StringConverter::toString(mPosition->get().x).c_str());
    pScpathPosition->SetAttribute("y", Ogre::StringConverter::toString(mPosition->get().y).c_str());
    pScpathPosition->SetAttribute("z", Ogre::StringConverter::toString(mPosition->get().z).c_str());

	return pScpath;
}

void ScpathEditor::setParentImpl(CBaseEditor *oldparent, CBaseEditor *newparent) {
    if(!mHandle) {
        return;
	}
    if(oldparent) {
        oldparent->getNode()->removeChild(mHandle);
	}    
    if(newparent) {
        newparent->getNode()->addChild(mHandle);
	}
}

void ScpathEditor::setSelectedImpl(bool bSelected) {
    CBaseEditor::setSelectedImpl(bSelected);
    showBoundingBox(bSelected);
}

bool ScpathEditor::setHighlightedImpl(bool highlight) {
    showBoundingBox(highlight);
    return true;
}

void ScpathEditor::setDerivedPosition(Ogre::Vector3 val) {
    if (getParent()) {
        Ogre::Quaternion qParent = getParent()->getDerivedOrientation().Inverse();
        Ogre::Vector3 vParent = getParent()->getDerivedPosition();
        Ogre::Vector3 newPos = (val - vParent);
        val = qParent * newPos;
    }
    mPosition->set(val);
}

Ogre::Vector3 ScpathEditor::getDerivedPosition() {
    if (mHandle) {
        return mHandle->_getDerivedPosition();
	} else {
        return getParent()->getDerivedPosition() + (getParent()->getDerivedOrientation() * mPosition->get());
	}
}

Ogre::AxisAlignedBox ScpathEditor::getAABB() {
    if (mEntity) {
        return mEntity->getBoundingBox();
    } else {
        return Ogre::AxisAlignedBox::BOX_NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ScpathEditor::_setPosition(OgitorsPropertyBase* property, const Ogre::Vector3& position) {
	if (mHandle) {
		mHandle->setPosition(position);
    }
    _updatePaging();

    return true;
}

bool ScpathEditor::_setPath(OgitorsPropertyBase* property, const Ogre::String& shape) {
	return true;
}

bool ScpathEditor::_setEnabled(OgitorsPropertyBase* property, const bool& is_enabled) {
    return true;
}

bool ScpathEditor::_setUpdateCallEnabled(OgitorsPropertyBase* property, const bool& is_update_call_enabled) {
    return true;
}

void ScpathEditor::showBoundingBox(bool bShow) {
    if (!mBoxParentNode) {
        createBoundingBox();
	}

    if (mBBoxNode) {
        int matpos = 0;
        if (mHighlighted->get()) {
            ++matpos;
            if(mSelected->get()) {
                ++matpos;
			}
        }        
        mOBBoxRenderable->setMaterial(mOBBMaterials[matpos]);
        mBBoxNode->setVisible(( bShow || mSelected->get() || mHighlighted->get() ));
    }
}

bool ScpathEditor::load(bool async) {
	cout << mName->get() << endl;
    if (mLoaded->get()) {
		return true;
	}
    if(!getParent()->load()) {
        return false;
	}
    //Create Sound Object
    mEntity = mOgitorsRoot->GetSceneManager()->createEntity(mName->get(), "Scpath.mesh");

    mHandle = getParent()->getNode()->createChildSceneNode(mName->get());
    mHandle->attachObject(mEntity);

    mHandle->setPosition(mPosition->get());

    mLoaded->set(true);

    return true;
}

bool ScpathEditor::unLoad() {
    if (!mLoaded->get()) {
        return true;
	}
    destroyBoundingBox();

    if (mHandle) {
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

void ScpathEditor::createProperties(OgitorsPropertyValueMap &params) {
    PROPERTY_PTR(mIsEnabled, "enabled", bool, true, 0, SETTER(bool, ScpathEditor, _setEnabled));
    PROPERTY_PTR(mPosition , "position" , Ogre::Vector3, Ogre::Vector3::ZERO, 0, SETTER(Ogre::Vector3, ScpathEditor, _setPosition));
	PROPERTY_PTR(mPath, "path", Ogre::String, "", 0, SETTER(Ogre::String, ScpathEditor, _setPath));
    PROPERTY_PTR(mIsUpdateCallEnabled, "update_call_enabled", bool, false, 0, SETTER(bool, ScpathEditor, _setUpdateCallEnabled));
    mProperties.initValueMap(params);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

ScpathEditor::ScpathEditor(CBaseEditorFactory *factory): CBaseEditor(factory) {	  
	mHandle = 0;
    mPosition = 0;
    mPath = 0;
    mIsEnabled = 0;
    mIsUpdateCallEnabled = 0;
}

ScpathEditor::~ScpathEditor() {
}


//////////////////////////////////////////////////////////////////////////////////////////////////

ScpathEditorFactory::ScpathEditorFactory(OgitorsView *view) : CBaseEditorFactory(view) {
    mTypeName = "Scpath Object";
    mEditorType = ETYPE_NODE;
    mAddToObjectList = true;
    mRequirePlacement = true;
    mIcon = "Icons/Scpath.svg";//----------------
    mCapabilities = CAN_MOVE|CAN_DELETE|CAN_CLONE|CAN_FOCUS|CAN_DRAG|CAN_ACCEPTCOPY;

    AddPropertyDefinition("enabled", "Enabled", "If this component is enabled.", PROP_BOOL);
    AddPropertyDefinition("position","Position", "The position of the object.", PROP_VECTOR3);
	AddPropertyDefinition("path", "Script Path", "The path of the script", PROP_STRING);
    AddPropertyDefinition("update_call_enabled", "Update_Call_Enabled", "If the onUpdate function which will be called every frame of this script is enabled.", PROP_BOOL);
}

CBaseEditorFactory *ScpathEditorFactory::duplicate(OgitorsView *view) {
    CBaseEditorFactory *ret = OGRE_NEW ScpathEditorFactory(view);
    ret->mTypeID = mTypeID;

    return ret;
}

CBaseEditor *ScpathEditorFactory::CreateObject(CBaseEditor **parent, OgitorsPropertyValueMap &params) {
    ScpathEditor *object = OGRE_NEW ScpathEditor(this);
    OgitorsPropertyValueMap::iterator ni;

    if ((ni = params.find("init")) != params.end()) {
        OgitorsPropertyValue value = EMPTY_PROPERTY_VALUE;
        value.val = Ogre::Any(CreateUniqueID("ScriptPath"));
        params["name"] = value;
        params.erase(ni);
    }

    object->createProperties(params);
    object->mParentEditor->init(*parent);
    object->registerForPostSceneUpdates();

    mInstanceCount++;
    return object;
}

Ogre::String ScpathEditorFactory::GetPlaceHolderName() {
    return "Scpath.mesh";
}
/////////////////////////////////////////////////////////////////////////
ScpathEditorFactory *fasoundFac = 0;


bool dllStartPlugin(void *identifier, Ogre::String& name) {
    name = "Scpath Object";
    fasoundFac = OGRE_NEW ScpathEditorFactory();
    OgitorsRoot::getSingletonPtr()->RegisterEditorFactory(identifier, fasoundFac);
    return true;
}

bool dllStopPlugin(void) {
    OGRE_DELETE fasoundFac;
    fasoundFac = 0;
    return true;
}
