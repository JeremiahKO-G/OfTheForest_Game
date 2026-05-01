#include "quadtree.h"
#include "game_object.h"

bool AABB::contains(Vec<float> point) const { // review with prof how this works
    Vec<float> displacement = point - center;
    return std::abs(displacement.x) < half_dimension.x && std::abs(displacement.y) < half_dimension.y;
}

bool AABB::intersects(AABB other) const {
    Vec<float> displacement = other.center - center;
    return std::abs(displacement.x) < (half_dimension.x + other.half_dimension.x)
        && std::abs(displacement.y) < (half_dimension.y + other.half_dimension.y);
}

Quadtree::Quadtree(AABB boundary)
    : boundary{boundary} {}

void Quadtree::clear() {
    objects.clear();
    ne = nullptr;
    nw = nullptr;
    se = nullptr;
    sw = nullptr;
}

bool Quadtree::insert(GameObject* obj) {
    // ignore objects not in this node
    if (!boundary.contains(obj->get_bounding_box().center)) {
        return false;
    }

    // if there is space, add it
    if (objects.size() < NODE_CAPACITY && nw == nullptr) {
        objects.push_back(obj);
        return true;
    }

    // otherwise subdivide and insert into children
    if (nw == nullptr) {
        subdivide();
    }

    return nw->insert(obj) || ne->insert(obj) || sw->insert(obj) || se->insert(obj);
}

std::vector<GameObject*> Quadtree::query_range(AABB range) const {
    if (!boundary.intersects(range)) {
        return {};
    }

    // handle leaf node
    std::vector<GameObject*> results;
    if (nw == nullptr) {
        // check each object (the range of them) and whether they intersect with the player
        std::copy_if(std::begin(objects), std::end(objects), std::back_inserter(results),
                [&](GameObject* obj) {return range.intersects(obj->get_bounding_box());}
        );
        return results;
    }

    // add objects from children
    auto pts = nw->query_range(range);
    results.insert(std::end(results), std::begin(pts), std::end(pts));
    pts = ne->query_range(range);
    results.insert(std::end(results), std::begin(pts), std::end(pts));
    pts = sw->query_range(range);
    results.insert(std::end(results), std::begin(pts), std::end(pts));
    pts = se->query_range(range);
    results.insert(std::end(results), std::begin(pts), std::end(pts));

    return results;
}

void Quadtree::subdivide() {
    Vec<float> half = boundary.half_dimension / 2.0f;
    // north is actually south and vice versa
    nw = std::make_shared<Quadtree>(AABB{{boundary.center.x - half.x, boundary.center.y - half.y}, half});
    ne = std::make_shared<Quadtree>(AABB{{boundary.center.x + half.x, boundary.center.y - half.y}, half});
    sw = std::make_shared<Quadtree>(AABB{{boundary.center.x - half.x, boundary.center.y + half.y}, half});
    se = std::make_shared<Quadtree>(AABB{{boundary.center.x + half.x, boundary.center.y + half.y}, half});

    for (auto obj : objects) {
        insert(obj);
    }
    objects.clear();
}

