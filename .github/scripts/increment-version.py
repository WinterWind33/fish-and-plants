# Copyright (C) 2022 Andrea Ballestrazzi
import sys

# Represents the version of our component (component = sub software)
class ComponentVersion:
    def __init__(self, major, minor, patch):
        self.major = major
        self.minor = minor
        self.patch = patch

    def incrementMajor(self, n):
        self.major += n

    def incrementMinor(self, n):
        self.minor += n

    def incrementPatch(self, n):
        self.patch += n

    def getVersionNumbers(self):
        return [self.major, self.minor, self.patch]

class ConfigurationFileController:
    def __init__(self, configFileName, componentName):
        self.fileName = configFileName
        self.componentName = componentName
        self.fileLines = []

        # The following attributes represent the lines where
        # the macros are defined inside the given file.
        self.majorMacroDefLine = -1
        self.minorMacroDefLine = -1
        self.patchMacroDefLine = -1

    def loadVersionNumbers(self) -> ComponentVersion:
        # 1. We read the lines from the file.
        with open(self.fileName, "r") as inputFile:
            self.fileLines = inputFile.readlines()

        inputFile.close()

        # Now that we have the lines we can start searching for the string
        # that represents our macros. Here we create the names of the macros
        # that we need to find.
        major_macro_name = f"{self.componentName}_VERSION_MAJOR"
        minor_macro_name = f"{self.componentName}_VERSION_MINOR"
        patch_macro_name = f"{self.componentName}_VERSION_PATCH"

        version_major = -1
        version_minor = -1
        version_patch = -1

        current_file_line = 0
        for line in self.fileLines:
            # We split up the line so we can find if some string matches the
            # expected one.
            words = line.split(" ")

            # We expect something like:
            # cpp: #define <COMP_NAME>_VERSION_MAJOR 3
            if len(words) >= 3:
                if(words[1] == major_macro_name):
                    version_major = int(words[2])
                    self.majorMacroDefLine = current_file_line

                if(words[1] == minor_macro_name):
                    version_minor = int(words[2])
                    self.minorMacroDefLine = current_file_line

                if(words[1] == patch_macro_name):
                    version_patch = int(words[2])
                    self.patchMacroDefLine = current_file_line

            current_file_line += 1

        return ComponentVersion(version_major, version_minor, version_patch)

    def commitNewVersion(self, newVersion):
        [major, minor, patch] = newVersion.getVersionNumbers()

        # Before writing the new version inside the file we need
        # to update the lines with the new version numbers.
        self.fileLines[self.majorMacroDefLine] = f"#define {self.componentName}_VERSION_MAJOR {major}\n"
        self.fileLines[self.minorMacroDefLine] = f"#define {self.componentName}_VERSION_MINOR {minor}\n"
        self.fileLines[self.patchMacroDefLine] = f"#define {self.componentName}_VERSION_PATCH {patch}\n"

        with open(self.fileName, "w") as configFile:
            configFile.writelines(self.fileLines)

        configFile.close()

# In this case:
#   - argv[1] = Component configuration file.
#   - argv[2] = Component name.
#   - argv[2..N] = versions number to update. Could be:
#       1. -M = increment major number.
#       2. -m = increment minor number.
#       3. -p = increment patch number.
# For now, the file should have he following macros for the versions numbers:
#   - <COMP_NAME>_VERSION_MAJOR
#   - <COMP_NAME>_VERSION_MINOR
#   - <COMP_NAME>_VERSION_PATCH
# If no option is given, then the patch is automatically increased.
def IncrementVersion(argv) -> ComponentVersion:
    assert len(argv) >= 3, "You need to provide at least the configuration file and the component name."
    bIncrementMajor = False
    bIncrementMinor = False
    bIncrementPatch = False

    # Here we check which version number we need to increment
    if len(argv) > 3:
        for i in range(3, len(argv)):
            bIncrementMajor = (bIncrementMajor or (argv[i] == "-M"))
            bIncrementMinor = (bIncrementMinor or (argv[i] == "-m"))
            bIncrementPatch = (bIncrementPatch or (argv[i] == "-p"))

    # If none of the above flags are checked, we increment the patch number by default.
    if not bIncrementMajor and not bIncrementMinor and not bIncrementPatch:
        bIncrementPatch = True

    configFileController = ConfigurationFileController(argv[1], argv[2])

    # Now we load the version from the file controller
    compCurrentVersion = configFileController.loadVersionNumbers()

    # Now that we have the component we can increment the version
    if bIncrementMajor:
        compCurrentVersion.incrementMajor(1)
    if bIncrementMinor:
        compCurrentVersion.incrementMinor(1)
    if bIncrementPatch:
        compCurrentVersion.incrementPatch(1)

    # And now we can write out the changes to the config file.
    configFileController.commitNewVersion(compCurrentVersion)

    return compCurrentVersion

if __name__ == "__main__":
    [newMajor, newMinor, newPatch] = IncrementVersion(sys.argv).getVersionNumbers()

    print(f"[INFO] => Increment {sys.argv[2]} version to {newMajor}.{newMinor}.{newPatch}.")
    print(f"[INFO] => Version changes have been written inside {sys.argv[1]}.")