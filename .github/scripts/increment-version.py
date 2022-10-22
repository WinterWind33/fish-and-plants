# Copyright (C) 2022 Andrea Ballestrazzi
import sys
import copy
import os

# Represents the version of our component (component = sub software)
class ComponentVersion:
    def __init__(self, major, minor, patch):
        self.major = major
        self.minor = minor
        self.patch = patch

    def incrementMajor(self, n):
        self.major += n
        self.minor = 0
        self.patch = 0

    def incrementMinor(self, n):
        self.minor += n
        self.patch = 0

    def incrementPatch(self, n):
        self.patch += n

    def getVersionNumbers(self):
        return [self.major, self.minor, self.patch]

    def equals(self, otherVersion):
        return self.major == otherVersion.major and self.minor == otherVersion.minor and self.patch == otherVersion.patch

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
def IncrementVersion(argv, beforeVersion) -> ComponentVersion:
    assert len(argv) >= 3, "You need to provide at least the configuration file and the component name."
    bIncrementMajor = False
    bIncrementMinor = False
    # The default option is to increment the patch version
    bIncrementPatch = True

    # Here we check which version number we need to increment.
    for i in range(3, len(argv)):
        if argv[i] == "--major":
            bIncrementMajor = True

        if argv[i] == "--minor":
            bIncrementMinor = True

        if argv[i] == "--patch":
            bIncrementPatch = True

    newVersion = copy.deepcopy(beforeVersion)

    # Now that we have the component we can increment the version. The order
    # of update is given by the following order:
    # - Major: if the user wants the major to be update then we need
    # to update the major version but we also need to reset
    # the minor and the patch.
    # - Minor: if the user wants to update the minor version we need to
    # reset the patch version.
    if bIncrementMajor:
        newVersion.incrementMajor(1)
    if bIncrementMinor:
        newVersion.incrementMinor(1)
    if bIncrementPatch:
        newVersion.incrementPatch(1)

    return newVersion

# Usage:
# python3 increment-version.py <configFileName> <component-name> [--major, --minor, (--patch)]
if __name__ == "__main__":
    print(f"[INFO] => Python script working dir: {os.getcwd()}")

    # We need to load up the current version before incrementing it.
    configFileController = ConfigurationFileController(sys.argv[1], sys.argv[2])
    beforeVersion = configFileController.loadVersionNumbers()
    [beforeMajor, beforeMinor, beforePatch] = beforeVersion.getVersionNumbers()

    # If we get -1.-1.-1 we had a problem loading the numbers.
    assert beforeMajor >= 0 and beforeMinor >= 0 and beforePatch >= 0

    print(f"[INFO] => Found version {beforeMajor}.{beforeMinor}.{beforePatch}.")
    # Now we can increment the version according to the desired given options.

    incrementedVersion = IncrementVersion(sys.argv, beforeVersion)
    if incrementedVersion.equals(beforeVersion):
        print(f"[ERROR] => Could not increment the version. Skipping changes commit.")
        exit(1)

    configFileController.commitNewVersion(incrementedVersion)
    [afterMajor, afterMinor, afterPatch] = incrementedVersion.getVersionNumbers()

    print(f"[INFO] => Incremented {sys.argv[2]} version to {afterMajor}.{afterMinor}.{afterPatch}.")
    print(f"[INFO] => Version changes have been written to {sys.argv[1]}.")

    # Now we save the version string into a file so we can retrieve it later
    # for the git commit.
    with open("./tmp/new_version.txt", "a") as newVersionFile:
        newVersionFile.write(f"{afterMajor}.{afterMinor}.{afterPatch}\n")

    newVersionFile.close()

    print("[INFO] => The new version has been written to new_version.txt file.")
