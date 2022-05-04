#include <IOKit/IOKitLib.h>
#include <iostream>
#include <pqrs/osx/iokit_iterator.hpp>

namespace {
void print_class_name(const pqrs::osx::iokit_object_ptr& object_ptr) {
  io_iterator_t it = IO_OBJECT_NULL;
  pqrs::osx::kern_return r = IORegistryEntryGetChildIterator(*object_ptr, kIOServicePlane, &it);
  if (r) {
    pqrs::osx::iokit_iterator iokit_iterator(it);

    while (true) {
      auto next = iokit_iterator.next();
      if (!next) {
        break;
      }

      if (auto class_name = next.class_name()) {
        std::cout << "class_name: " << *class_name << std::endl;
      }

      print_class_name(next);
    }
  }
}
} // namespace

int main(void) {
  auto entry = IORegistryGetRootEntry(kIOMainPortDefault);
  print_class_name(pqrs::osx::iokit_object_ptr(entry));

  return 0;
}
