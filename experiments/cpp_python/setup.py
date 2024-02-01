from distutils.core import setup, Extension
import os


def get_cpp():
    files = []
    for directory in ["src", "python"]:
        files.extend([f"{directory}/{path}" for path in os.listdir(directory) if path.endswith(".cpp")])
    return files


def main():
    setup(name="dma_cma",
          version="1.0.0",
          description="Direct Memory Access to Contiguous Memory Allocation module",
          ext_modules=[Extension("dma_cma", get_cpp(), include_dirs=[
              "include"
          ])])


if __name__ == "__main__":
    main()
