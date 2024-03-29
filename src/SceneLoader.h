#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H
#pragma once


#include <Scene/Scene.hpp>
#include <Scene/Node.hpp>

#include <string>

#include <QtXml/QtXml>

using namespace std;
using namespace dt;

#define SL_TRUE "true"
#define SL_FALSE "false"

#define SL_SCENE "scene"
#define SL_NAME "name"
#define SL_NODES "nodes"
#define SL_NODE "node"
#define SL_SCALE "scale"
#define SL_POS "position"
#define SL_X "x"
#define SL_Y "y"
#define SL_Z "z"
#define SL_ROT "rotation"
#define SL_QW "qw"
#define SL_QX "qx"
#define SL_QY "qy"
#define SL_QZ "qz"
#define SL_COLOUR_R "r"
#define SL_COLOUR_G "g"
#define SL_COLOUR_B "b"
#define SL_COLOUR_A "a"

#define SL_COMPONENT_ENABLED "enabled"

#define SL_MESH_HANDLE "meshFile"
#define SL_MESH_ENTITY "entity"
#define SL_MESH_ENTITY_INDEX "index"
#define SL_MESH_ENTITY_MATERIAL_NAME "materialName"
#define SL_MESH_PLANE "plane"
#define SL_MESH_PLANE_MATERIAL "material"
#define SL_MESH_PLANE_SIZEX "height"
#define SL_MESH_PLANE_SIZEY "width"
#define SL_MESH_PLANE_NORMAL "normal"
#define SL_MESH_PLANE_ENABLE_NORMALS "hasNormals"
#define SL_MESH_PLANE_SEGMENTSX "xSegments"
#define SL_MESH_PLANE_SEGMENTSY "ySegments"
#define SL_MESH_PLANE_NUMTEXCOORD "numTexCoordSets"
#define SL_MESH_PLANE_UTILE "uTile"
#define SL_MESH_PLANE_VTILE "vTile"

#define SL_LIGHT "light"
#define SL_LIGHT_TYPE "type"
#define SL_LIGHT_TYPE_POINT "point"
#define SL_LIGHT_TYPE_DIRECTIONAL "directional"
#define SL_LIGHT_TYPE_SPOT "spot"
#define SL_LIGHT_DIFFUSE "colourDiffuse"
#define SL_LIGHT_SPECULAR "colourSpecular"
#define SL_LIGHT_ATTENUATION "lightAttenuation"
#define SL_LIGHT_ATTENUATION_RANGE "range"
#define SL_LIGHT_ATTENUATION_CONSTANT "constant"
#define SL_LIGHT_ATTENUATION_LINEAR "linear"
#define SL_LIGHT_ATTENUATION_QUADRATIC "quadratic"
#define SL_LIGHT_RANGE "lightRange"
#define SL_LIGHT_RANGE_INNER "inner"
#define SL_LIGHT_RANGE_OUTER "outer"
#define SL_LIGHT_RANGE_FALLOFF "falloff"
#define SL_LIGHT_DIRECTION "directionVector"

#define SL_CAST_SHADOWS "castShadows"

#define SL_CAMERA "camera"
//#define SL_CAMERA_VIEW_MODE "viewMode"
#define SL_CAMERA_POLYGON_MODE "polyMode"
#define SL_CAMERA_FOV "fov"
#define SL_CAMERA_CLIPPING "clipping"
#define SL_CAMERA_CLIPPING_NEAR "near"
#define SL_CAMERA_CLIPPING_FAR "far"

#define SL_SCPATH "scpath" /* script path */
#define SL_SCPATH_PATH "path"
#define SL_SCPATH_UPDATE_ENABLED "updateCallEnabled"

#define SL_MUSIC "music"
#define SL_MUSIC_PATH "musicPath"
#define SL_MUSIC_VOLUME "volume"

#define SL_SOUND "sound"
#define SL_SOUND_PATH "soundFile"
#define SL_SOUND_VOLUME "volume"

#define SL_INTERACTOR "interactor"
#define SL_INTERACTOR_RANGE "range"
#define SL_INTERACTOR_OFFSET "offset"
#define SL_INTERACTOR_INTERVAL "interval"
#define SL_INTERACTOR_TYPE "type"
#define SL_INTERACTOR_TYPE_RAYCASTING "RayCasting"
#define SL_INTERACTOR_TYPE_COLLISION "Collision"

#define SL_PHYSICS "physics"
#define SL_PHYSICS_SHAPE "shape"
#define SL_PHYSICS_SHAPE_BOX "BOX"
#define SL_PHYSICS_SHAPE_CYLINDER "CYLINDER"
#define SL_PHYSICS_SHAPE_SPHERE "SPHERE"
#define SL_PHYSICS_SHAPE_TRIMESH "TRIMESH"
#define SL_PHYSICS_RESMOVE "restrictMove"
#define SL_PHYSICS_RESROTATE "restrictRotate"
#define SL_PHYSICS_GRAVITY "gravity"
#define SL_PHYSICS_MASS "mass"
#define SL_PHYSICS_MESH_COM_NAME "meshComponentName"

//#define SL_BEGIN "terrain"

class SceneLoader 
{
public:
    /**
      * loads a scene.
      * @param path The path to the scene.
      * @returns A pointer to the scene or null when it fails.
      */
    static Scene* loadScene(QString path);

protected:
    static Node::NodeSP __loadElement(const QDomElement& og_element, Node::NodeSP dt_node = nullptr);

    static Node::NodeSP __loadNode(const QDomElement& og_node, Node::NodeSP dt_parent);

    static Node::NodeSP __loadMesh(const QDomElement& og_component, Node::NodeSP dt_node);

    static Node::NodeSP __loadLight(const QDomElement& og_component, Node::NodeSP dt_node);
     
    static Node::NodeSP __loadCamera(const QDomElement& og_component, Node::NodeSP dt_node);

    static Node::NodeSP __loadScriptPath(const QDomElement& og_component, Node::NodeSP dt_node);

    static Node::NodeSP __loadSound(const QDomElement& og_component, Node::NodeSP dt_node);

    static Node::NodeSP __loadMusic(const QDomElement& og_component, Node::NodeSP dt_node);

    static Node::NodeSP __loadInteractor(const QDomElement& og_component, Node::NodeSP dt_node);

    static Node::NodeSP __loadPhysics(const QDomElement& og_component, Node::NodeSP dt_node);

    static Scene* mScene;
};

#endif /* SCENE_LOADER_H */
