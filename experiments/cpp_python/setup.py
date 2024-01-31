from distutils.core import setup, Extension

def main():
    setup(name="answer",
          version="1.0.0",
          description="Answer module",
          ext_modules=[Extension("answer", ["test.cpp"])])

if __name__ == "__main__":
    main()