// scene.cpp

#include "scene.hpp"
#include "abstract_object.hpp"

levi::scene::scene() : is_visible_{true}, is_updateble_{true} {}

levi::scene::~scene() {}

void levi::scene::add_item(std::shared_ptr<abstract_object> obj) {
  obj->set_scene(this);
  item_list_.push_back(obj);
}

const levi::item_list &levi::scene::get_item_list() const { return item_list_; }

void levi::scene::update() {
  for (auto item = item_list_.begin(); item != item_list_.end();) {
    if ((*item)->is_for_delete()) {
      item = item_list_.erase(item);
    } else {
      (*item)->update();
      ++item;
    }
  }
}

std::list<levi::abstract_object *>
levi::scene::get_collisions_for(levi::rect rect) {
  std::list<levi::abstract_object *> retlist;
  for (auto item = item_list_.begin(); item != item_list_.end(); ++item) {
    if (is_crossing((*item)->get_rectangle(), rect)) {
      retlist.push_back(item->get());
    }
  }
  return retlist;
}

levi::id_state levi::scene::get_id() const { return id_state::none; }

void levi::render(engine &engine, const scene &scene) {
  auto &item_list = scene.get_item_list();
  for (auto &item : item_list) {
    levi::draw(engine, *item.get());
  }
}

void levi::scene::set_visibility(bool value) { is_visible_ = value; }
bool levi::scene::is_visible() { return is_visible_; }
void levi::scene::set_updatebility(bool value) { is_updateble_ = value; }
bool levi::scene::is_updateble() { return is_updateble_; }
