@0x88f62a6b166674a9;

struct Size {
  width @0 :UInt16;
  height @1 :UInt16;
  depth @2 :UInt8;
}

struct Image {
  size @0 :Size;
  raw @1 :List(UInt8);
}
