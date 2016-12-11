//
// Created by Hazen on 12/10/2016.
//

#ifndef LD37_MAP_H
#define LD37_MAP_H

#include <Urho3D/Core/Object.h>
#include <Urho3D/Urho2D/Drawable2D.h>

namespace Urho3D
{
    class Node;
    class Scene;
    class NavigationMesh;
}

namespace Ld37
{
    /// Minimum size of a generated map
    const int MIN_MAP_SIZE = 1;
    /// Maximum size of a generated map
    const int MAX_MAP_SIZE = 30;
    /// Density of checkpoints in the hero path relative to map size
    const float CHECKPOINT_DENSITY = .05;
    /// Density of item spawn relative to map size
    const float ITEM_DENSITY = .05;
    /// Size of a room tile
    const float TILE_SIZE = 128 * Urho3D::PIXEL_SIZE;
    /// Size of a room
    const float ROOM_SIZE = TILE_SIZE * 4;
    /// Name of a closed room tile
    const Urho3D::String CLOSED_ROOM = "Closed";
    /// Name of a open room tile
    const Urho3D::String OPEN_ROOM = "Open";
    /// Name of a floor room tile
    const Urho3D::String FLOOR = "Floor";

    /// Stores information about items in a room
    struct Item {
        enum Type {
            TREASURE, HOLE, HERO_SPAWNER, MONSTER_SPAWNER, EXIT, FALSE_TREASURE
        } type;
        enum Direction {
            CENTER = 0, NORTH, EAST, SOUTH, WEST
        } dir;
        Urho3D::Vector2 pos;
        bool triggered = false;
    };

    /// Stores information about rooms in a map
    struct Space {
        enum Type {
            ROOM, EMPTY, SPAWN
        } type;
        Urho3D::Vector2 pos;
        Space* next = NULL;
        Space* prev = NULL;
        Urho3D::Vector<Item> items;
    };
    /// Generates and contains all logic concerning the level map
    class Map : public Urho3D::Object {
        URHO3D_OBJECT(Map, Urho3D::Object);

    public:
        /// Constructor
        Map(Urho3D::Context* context);

        /// Destructor
        virtual ~Map();

        /// Generates a new map and returns the root node of the result
        Urho3D::Node* Generate(Urho3D::Scene* scene_);

        /// Temp path debugger
        Urho3D::PODVector<Urho3D::Vector3> path_;

    private:
        /// Generates a map
        Urho3D::Vector<int> GenerateMap();

        /// Populates a map with items
        void PopulateMap(Urho3D::Vector<int>& map);

        /// Constructs a pre-generated map
        Urho3D::Node* ConstructMap(Urho3D::Scene* scene);

        /// Generates a point on the side of map
        Urho3D::IntVector2 GenerateSidePoint(Urho3D::IntVector2 size, int side);

        /// Calculates the shortest path between a start and an end point
        /// Returns false if no path is found
        bool AStar(Urho3D::Vector<int> &map, Urho3D::IntVector2 size,
            Urho3D::IntVector2 start, Urho3D::IntVector2 end,
            Urho3D::Vector <Urho3D::IntVector2>& path);

        /// Constructs a path based on the output of the AStar algorithm
        /// A utility function called by AStar in the final step
        void ConstructPath(const Urho3D::HashMap<Urho3D::IntVector2, Urho3D::IntVector2>& mappings,
            Urho3D::IntVector2 current,
            Urho3D::Vector<Urho3D::IntVector2>& path);

        /// The application context
        Urho3D::Context* context_;

        /// The map data of the map
//        Urho3D::Vector<int> map_;
        Urho3D::Vector<Space> map_;
        Urho3D::IntVector2 size_;

        int heroSpawn_;
        int heroExit_;
        int playerSpawn_;
        int heroPathLength_;

        /// List of nodes that make up the map
        Urho3D::Node* mapNode_;
        Urho3D::NavigationMesh* navMesh_;

    };
}


#endif //LD37_MAP_H
