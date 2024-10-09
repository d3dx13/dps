@0x88f62a6b166674a9;

struct Size {
  width @0 :UInt16;
  height @1 :UInt16;
  depth @2 :UInt8;
}

struct Image {
  raw @0 :List(UInt8);
  size @1 :Size;
}
