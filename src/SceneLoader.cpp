#include "SceneLoader.h"

#include "Graphics/CameraComponent.hpp"
#include "Graphics/MeshComponent.hpp"
#include "Graphics/LightComponent.hpp"
#include "Audio/SoundComponent.hpp"
#include "Audio/MusicComponent.hpp"
#include "Logic/ScriptComponent.hpp"
#include "Logic/InteractionComponent.hpp"
#include "Logic/RaycastComponent.hpp"
#include "Logic/CollisionComponent.hpp"
#include "Core/ResourceManager.hpp"


#include <QtXml/QtXml>
#include <OgreProcedural.h>
#include <OGRE/OgreSubEntity.h>

#include <cstdint>

Scene* SceneLoader::mScene = nullptr;

Scene* SceneLoader::loadScene(QString path)
{
    mScene = nullptr;
    QFile file(path);
    QDomDocument doc;
    if ( !file.open(QIODevice::ReadOnly) )
    {
        dt::Logger::get().error("Couldn't open file " + path);
        return nullptr;
    }

    cout << "loading result: " << endl;
    if ( doc.setContent(&file) )
    {
        mScene = new Scene(path);
        OgreProcedural::Root::getInstance()->sceneManager = mScene->getSceneManager();

        QDomElement root = doc.documentElement();

        for ( QDomElement scene_child = root.firstChildElement();
            !scene_child.isNull(); scene_child = scene_child.nextSiblingElement() )
        {
            if ( scene_child.nodeName() != SL_NODES ) //For free components(not including free mesh components).
            {
                __loadElement(scene_child);
            }
            else //For nodes and free mesh components.
            {
                for ( QDomElement nodes_child = scene_child.firstChildElement();
                    !nodes_child.isNull(); nodes_child = nodes_child.nextSiblingElement() )
                {
                    __loadElement(nodes_child);
                }
            }
        }
    }

    return mScene;
}

Node::NodeSP SceneLoader::__loadElement(const QDomElement& og_element, Node::NodeSP dt_node)
{    
    QString name = og_element.nodeName();
    Node::NodeSP node = nullptr;

    if ( name == SL_LIGHT )
    {
        node = __loadLight(og_element, dt_node);                   //Light
    }
    else if ( name == SL_CAMERA )
    {
        node = __loadCamera(og_element, dt_node);                  //Camera
    }
    else if ( name == SL_SOUND )
    {
        node = __loadSound(og_element, dt_node);                   //Sound
    }
    else if ( name == SL_MUSIC )
    {
        node = __loadMusic(og_element, dt_node);                   //Music
    }
    else if ( name == SL_SCRIPT_PATH )
    {
        node = __loadScriptPath(og_element, dt_node);              //ScriptPath
    }
    else if ( name == SL_INTERACTOR )
    {
        node = __loadInteractor(og_element, dt_node);              //Interactor
    }
    else if ( name == SL_PHYSICS )
    {
        node = __loadPhysics(og_element, dt_node);                 //Physics
    }
    else if ( name == SL_NODE )
    {
        if ( og_element.firstChildElement(SL_MESH_ENTITY).isNull() && og_element.firstChildElement(SL_MESH_PLANE).isNull() )
        {
            node = __loadNode(og_element, dt_node);                //Node
        }
        else
        {
            node = __loadMesh(og_element, dt_node);                //Mesh
        }
    }

    return node;
}

Node::NodeSP SceneLoader::__loadNode(const QDomElement& og_node, Node::NodeSP dt_parent)
{
    Node::NodeSP node = nullptr;

    if ( !og_node.isNull() )
    {
        node = Node::NodeSP(new Node(og_node.attribute(SL_NAME)));

        if ( dt_parent )
        {
            dt_parent->addChildNode(node.get());
        }
        else
        {
            mScene->addChildNode(node.get());
        }

        QDomElement pos = og_node.firstChildElement(SL_POS);
        QDomElement rot = og_node.firstChildElement(SL_ROT);
        QDomElement scale = og_node.firstChildElement(SL_SCALE);

        node->setPosition(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(),
            pos.attribute(SL_Z).toFloat());
        node->setRotation(Ogre::Quaternion(rot.attribute(SL_QW).toFloat(),
            rot.attribute(SL_QX).toFloat(), rot.attribute(SL_QY).toFloat(), rot.attribute(SL_QZ).toFloat()));
        node->setScale(Ogre::Vector3(scale.attribute(SL_X).toFloat(), scale.attribute(SL_Y).toFloat(),
            scale.attribute(SL_Z).toFloat()));

        for ( QDomElement node_child = scale.nextSiblingElement();
            !node_child.isNull(); node_child = node_child.nextSiblingElement() )
        {
            __loadElement(node_child, node);
        }
    }

    return node;
}

Node::NodeSP SceneLoader::__loadMesh(const QDomElement& og_component, Node::NodeSP dt_node)
{
    Node::NodeSP node = dt_node;

    if ( !og_component.isNull() )
    {
        QString name = og_component.attribute(SL_NAME);

        if ( node == nullptr )
        {
            node = mScene->addChildNode(new Node(name + "_node"));

            QDomElement pos = og_component.firstChildElement(SL_POS);
            QDomElement rot = og_component.firstChildElement(SL_ROT);
            QDomElement scale = og_component.firstChildElement(SL_SCALE);

            node->setPosition(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(),
                pos.attribute(SL_Z).toFloat());
            node->setRotation(Ogre::Quaternion(rot.attribute(SL_QW).toFloat(),
                rot.attribute(SL_QX).toFloat(), rot.attribute(SL_QY).toFloat(), rot.attribute(SL_QZ).toFloat()));
            node->setScale(Ogre::Vector3(scale.attribute(SL_X).toFloat(), scale.attribute(SL_Y).toFloat(),
                scale.attribute(SL_Z).toFloat()));
        }        

        //entity
        QDomElement unknown_mesh = og_component.firstChildElement(SL_SCALE).nextSiblingElement();
        if ( unknown_mesh.nodeName() == SL_MESH_ENTITY )
        {
            const QDomElement& entity = unknown_mesh;

            std::tr1::shared_ptr<MeshComponent> mesh = node->addComponent<MeshComponent>(
                new MeshComponent(entity.attribute(SL_MESH_HANDLE), "", entity.attribute(SL_NAME))
                );

            for ( QDomElement mat = entity.firstChildElement(); !mat.isNull(); mat = mat.nextSiblingElement() )
            {
                QString material_handle = mat.attribute(SL_MESH_ENTITY_MATERIAL_NAME);
                uint32_t index = mat.attribute(SL_MESH_ENTITY_INDEX).toUInt();

                mesh->getOgreEntity()->getSubEntity(index)->setMaterialName(material_handle.toStdString());
            }

            mesh->setCastShadows(entity.attribute(SL_CAST_SHADOWS).toInt());
        }
        else if ( unknown_mesh.nodeName() == SL_MESH_PLANE )
        {
            //plane
            const QDomElement& plane = unknown_mesh;
            if ( !plane.isNull() )
            {
                //create mesh
                OgreProcedural::PlaneGenerator()
                    .setSizeX(plane.attribute(SL_MESH_PLANE_SIZEX).toFloat())
                    .setSizeY(plane.attribute(SL_MESH_PLANE_SIZEY).toFloat())
                    .setEnableNormals(plane.attribute(SL_MESH_PLANE_ENABLE_NORMALS).toInt())
                    .setNumSegX(plane.attribute(SL_MESH_PLANE_SEGMENTSX).toInt())
                    .setNumSegY(plane.attribute(SL_MESH_PLANE_SEGMENTSY).toInt())
                    .setNumTexCoordSet(plane.attribute(SL_MESH_PLANE_NUMTEXCOORD).toInt())
                    .setUTile(plane.attribute(SL_MESH_PLANE_UTILE).toFloat())
                    .setVTile(plane.attribute(SL_MESH_PLANE_VTILE).toFloat())
                    .setNormal(Ogre::Vector3( plane.firstChildElement(SL_MESH_PLANE_NORMAL).attribute(SL_X).toFloat(),
                    plane.firstChildElement(SL_MESH_PLANE_NORMAL).attribute(SL_Y).toFloat(),
                    plane.firstChildElement(SL_MESH_PLANE_NORMAL).attribute(SL_Z).toFloat()))
                    .realizeMesh(plane.attribute(SL_NAME).toStdString());

                //add entity
                node->addComponent<MeshComponent>(
                    new MeshComponent(plane.attribute(SL_NAME), plane.attribute(SL_MESH_PLANE_MATERIAL),plane.attribute(SL_NAME))
                    );
            }
        }
    }

    return node;
}

Node::NodeSP SceneLoader::__loadLight(const QDomElement& og_component, Node::NodeSP dt_node)
{
    Node::NodeSP node = dt_node;

    if ( !og_component.isNull() )
    {
        QString name = og_component.attribute(SL_NAME);

        if ( node == nullptr )
        {
            node = mScene->addChildNode(new Node(name + "_node"));

            QDomElement pos = og_component.firstChildElement(SL_POS);
            QDomElement dir = og_component.firstChildElement(SL_LIGHT_DIRECTION);

            node->setPosition(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(),
                pos.attribute(SL_Z).toFloat());
            node->setDirection(Ogre::Vector3(dir.attribute(SL_X).toFloat(), dir.attribute(SL_Y).toFloat(),
                dir.attribute(SL_Z).toFloat()));
        }

        node->addComponent<LightComponent>(new LightComponent(name))->setCastShadows(
            og_component.attribute(SL_CAST_SHADOWS).toInt());
    }

    return node;
}

Node::NodeSP SceneLoader::__loadCamera(const QDomElement& og_component, Node::NodeSP dt_node)
{
    Node::NodeSP node = dt_node;

    if ( !og_component.isNull() )
    {
        QString name = og_component.attribute(SL_NAME);

        if ( node == nullptr )
        {
            node = mScene->addChildNode(new Node(name + "_node"));

            QDomElement pos = og_component.firstChildElement(SL_POS);
            QDomElement rot = og_component.firstChildElement(SL_ROT);

            node->setPosition(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(),
                pos.attribute(SL_Z).toFloat());
            node->setRotation(Ogre::Quaternion(rot.attribute(SL_QW).toFloat(),
                rot.attribute(SL_QX).toFloat(), rot.attribute(SL_QY).toFloat(), rot.attribute(SL_QZ).toFloat()));
        }

        node->addComponent<CameraComponent>(new CameraComponent(name));
    }

    return node;
}

Node::NodeSP SceneLoader::__loadScriptPath(const QDomElement& og_component, Node::NodeSP dt_node)
{
    Node::NodeSP node = dt_node;

    if ( !og_component.isNull() )
    {
        QString name = og_component.attribute(SL_NAME);

        if ( node == nullptr )
        {
            node = mScene->addChildNode(new Node(name + "_node"));

            QDomElement pos = og_component.firstChildElement(SL_POS);

            node->setPosition(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(),
                pos.attribute(SL_Z).toFloat());
        }

        node->addComponent<ScriptComponent>(new ScriptComponent(name));
    }

    return node;
}

Node::NodeSP SceneLoader::__loadSound(const QDomElement& og_component, Node::NodeSP dt_node)
{
    Node::NodeSP node = dt_node;

    if ( !og_component.isNull() )
    {
        QString name = og_component.attribute(SL_NAME);

        if ( node == nullptr )
        {
            node = mScene->addChildNode(new Node(name + "_node"));

            QDomElement pos = og_component.firstChildElement(SL_POS);

            node->setPosition(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(),
                pos.attribute(SL_Z).toFloat());
        }

        auto s = node->addComponent<SoundComponent>(new SoundComponent(og_component.attribute(SL_SOUND_NAME), name));
        
        s->setVolume(og_component.attribute(SL_SOUND_VOL).toFloat());
    }

    return node;
}

Node::NodeSP SceneLoader::__loadMusic(const QDomElement& og_component, Node::NodeSP dt_node)
{
    Node::NodeSP node = dt_node;

    if ( !og_component.isNull() )
    {
        QString name = og_component.attribute(SL_NAME);

        if ( node == nullptr )
        {
            node = mScene->addChildNode(new Node(name + "_node"));

            QDomElement pos = og_component.firstChildElement(SL_POS);

            node->setPosition(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(),
                pos.attribute(SL_Z).toFloat());
        }

        node->addComponent<MusicComponent>(new MusicComponent(name));
    }

    return node;
}

Node::NodeSP SceneLoader::__loadInteractor(const QDomElement& og_component, Node::NodeSP dt_node)
{
    Node::NodeSP node = dt_node;

    if ( !og_component.isNull() )
    {
        QString name = og_component.attribute(SL_NAME);

        if ( node == nullptr )
        {
            node = mScene->addChildNode(new Node(name + "_node"));

            QDomElement pos = og_component.firstChildElement(SL_POS);

            node->setPosition(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(),
                pos.attribute(SL_Z).toFloat());
        }

        QString s = og_component.attribute(SL_INTERACTOR_TYPE);
        if ( s == SL_INTERACTOR_TYPE_RAYCASTING )
        {
            node->addComponent<InteractionComponent>(new RaycastComponent(name));
        }
        else if ( s == SL_INTERACTOR_TYPE_COLLISION )
        {
            node->addComponent<InteractionComponent>(new CollisionComponent(name));
        }
    }

    return node;
}

Node::NodeSP SceneLoader::__loadPhysics(const QDomElement& og_component, Node::NodeSP dt_node)
{
    Node::NodeSP node = dt_node;
    if ( !og_component.isNull() )
    {
        QString name = og_component.attribute(SL_NAME);

        if ( node == nullptr )
        {
            node = mScene->addChildNode(new Node(name + "_node"));

            QDomElement pos = og_component.firstChildElement(SL_POS);
            QDomElement res_move = og_component.firstChildElement(SL_PHYSICS_RESMOVE);
            QDomElement res_rotate = og_component.firstChildElement(SL_PHYSICS_RESROTATE);
            QDomElement gravity = og_component.firstChildElement(SL_PHYSICS_GRAVITY);

            node->setPosition(pos.attribute(SL_X).toFloat(), pos.attribute(SL_Y).toFloat(),
                pos.attribute(SL_Z).toFloat());

            QString s = og_component.attribute(SL_PHYSICS_SHAPE);
            auto type = PhysicsBodyComponent::CONVEX;

            if ( s == SL_PHYSICS_SHAPE_BOX )
            {
                type = PhysicsBodyComponent::BOX;
            }
            else if ( s == SL_PHYSICS_SHAPE_CYLINDER )
            {
                type = PhysicsBodyComponent::CYLINDER;
            }
            else if ( s == SL_PHYSICS_SHAPE_SPHERE )
            {
                type = PhysicsBodyComponent::SPHERE;
            }
            else if ( s == SL_PHYSICS_SHAPE_TRIMESH )
            {
                type = PhysicsBodyComponent::TRIMESH;
            }

            auto physics = node->addComponent<PhysicsBodyComponent>(
                new PhysicsBodyComponent(og_component.attribute(SL_PHYSICS_MESH_COM_NAME),name,type)
                );

            physics->setRestrictMovement(res_move.attribute(SL_X).toFloat(),res_move.attribute(SL_Y).toFloat(),
                res_move.attribute(SL_Z).toFloat());
            physics->setRestrictRotation(res_rotate.attribute(SL_X).toFloat(),res_move.attribute(SL_Y).toFloat(),
                res_move.attribute(SL_Z).toFloat());
            physics->setMass(og_component.attribute(SL_PHYSICS_MASS).toFloat());
            physics->setGravity(gravity.attribute(SL_X).toFloat(),gravity.attribute(SL_Y).toFloat(),
                gravity.attribute(SL_Z).toFloat());

        }
    }

    return node;
}