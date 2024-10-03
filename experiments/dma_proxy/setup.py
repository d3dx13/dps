from distutils.core import setup, Extension
import os


def get_cpp():
    files = []
    for directory in ["src", "python"]:
        files.extend([f"{directory}/{path}" for path in os.listdir(directory) if path.endswith(".cpp")])
    return files


def main():
    setup(name="linux_proxy",
          version="1.0.0",
          description="linux_proxy",
          ext_modules=[Extension("linux_proxy", get_cpp(), include_dirs=[
              "include"
          ])])


if __name__ == "__main__":
    main()
