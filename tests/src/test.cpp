#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <pqrs/osx/iokit_iterator.hpp>
#include <pqrs/osx/iokit_types.hpp>

TEST_CASE("iokit_iterator") {
  {
    pqrs::osx::iokit_iterator iokit_iterator;
    REQUIRE(iokit_iterator.valid() == false);
  }

  {
    auto entry = IORegistryGetRootEntry(type_safe::get(pqrs::osx::iokit_mach_port::null));
    io_iterator_t it = IO_OBJECT_NULL;
    pqrs::osx::kern_return r = IORegistryEntryGetChildIterator(entry, kIOServicePlane, &it);
    REQUIRE(static_cast<bool>(r) == true);

    pqrs::osx::iokit_iterator iokit_iterator(it);
    REQUIRE(iokit_iterator.valid() == true);
  }
}

namespace {
pqrs::osx::iokit_iterator copy_constructor_test(void) {
  pqrs::osx::iokit_iterator result;
  return result;
}
} // namespace

TEST_CASE("iokit_iterator::copy_constructor_test") {
  copy_constructor_test();
}
