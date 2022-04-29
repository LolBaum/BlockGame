from ast import Assert
from dataclasses import replace
import os
import hashlib
# import File stuff?
# import colors?

##############################################################
### Based a build script by sivansh11 
### https://github.com/sivansh11/python-based-cpp-build-system
##############################################################


# Include Folders
SDL_include = "libs/SDL2/x86_64-w64-mingw32/include/SDL2"
glew_include = "libs/glew-2.1.0/include/GL"
glm_include = "libs/glm"
single_header_include = "libs/include"
my_includes = "include"
include_list = [SDL_include, glew_include, single_header_include, my_includes]
includes = ["-I" + path for path in include_list + [glm_include]]



# Lib Folders
SDL_lib = "E:\\Programmieren\\_GameDev\\Ourcraft\\libs\\SDL2\\x86_64-w64-mingw32\\lib"#"libs/SDL2/x86_64-w64-mingw32/lib"
glew_lib = "libs/glew-2.1.0/lib/Release/x64"
libs_list = [SDL_lib, glew_lib]
libs = ["-L" + path for path in libs_list]

link_libs = ["-lglew32",
                "-lmingw32",
                "-lSDL2main",
                "-lSDL2",
                "-lglew32",
                "-lopengl32",
                "-lWs2_32",
                "-lole32",
                "-lcomctl32",
                "-lgdi32",
                "-lcomdlg32",
                "-luuid",]

source_folder = "src"
search_path = include_list + [glm_include + "/glm"] + libs_list + [source_folder]


# Internal Building Infos
saved = "build/.python_build/filehash.txt"
o_folder = "build/build_o"
build_folder = "build"
executable_name = "mainPyBuild.exe"

flags = ["-fdiagnostics-color=always", "-g", "-std=c++11"]


def read_file(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()
    return lines

def get_file_hash_from_file(file_path):
    lines = read_file(file_path)
    file_hash = dict()
    for line in lines:
        name, hash = line.split(' ')
        hash = hash.replace('\n', '')
        file_hash[name] = hash
    return file_hash


def get_all_files(path_to_search):
    files = list()
    for path in os.listdir(path_to_search):
        if not os.path.isdir(path_to_search + '/' + path):
            files.append(path_to_search + '/' + path)
        else:
            deeper_files = get_all_files(path_to_search + '/' + path)
            for file in deeper_files:
                files.append(file)
    return files


def get_file_hash_form_path(paths_to_search):
    file_hash = dict()
    files_to_check = list()
    for path in paths_to_search:
        if os.path.isdir(path):
            for file in get_all_files(path):
                files_to_check.append(file)
        else:
            print(f"given path is not valid\n\t\tpaht: {path}")
            quit(1)
    for file_path in files_to_check:
        file_hash[file_path] = hashlib.md5(open(file_path, 'rb').read()).hexdigest()
    return file_hash


def save_file_hash(file_hash, path):
    lines = list()
    for file_path in file_hash.keys():
        #print(file_path)
        #print(file_hash[file_path])
        lines.append(file_path + ' ' + file_hash[file_path] + '\n')
    with open(path, 'w') as file:
        file.writelines(lines)

def get_dependencies_of_file(file_path):
    #try:
        with open(file_path, 'r', encoding="Latin-1") as file:
            lines = file.readlines()
            dependencies = list()
            for line in lines:
                if "#include " in line:
                    new_line = line.replace('\n', '')
                    new_line = new_line.replace('#include ', '')
                    new_line = new_line.replace('"', '')
                    new_line = new_line.replace('<', '')
                    new_line = new_line.replace('>', '')
                    dependencies.append(new_line)
        return dependencies
    #except Exception as e:
    #    print(f"ERROR in get_dependencies_of_file:\n{e}\n\t\tfile_path: {file_path}")
    #    raise(e)

     

def get_dependency_of_files_from_path(paths_to_search):
    filepath_dependency = dict()
    for path in paths_to_search:
        if os.path.isdir(path):
            for file in get_all_files(path):
                #if ".c" in file  or  ".cpp" in file:
                    dependencies = get_dependencies_of_file(file)
                    for dep_file in dependencies:
                        if dep_file not in filepath_dependency:
                            filepath_dependency[dep_file] = set([file])
                        else:
                            filepath_dependency[dep_file].add(file)
        else:
            print(f"given path is not valid\n\t\tpath: {path}")
            quit(1)
    return filepath_dependency


def ret(lis):
    strr = ''
    for i, v in enumerate(lis):
        strr += v
        if i < len(lis) - 1:
            strr += ' '
    return strr


class Builder:
    def __init__(self, path_to_search, file_path) -> None:
        self.new_file_hash = get_file_hash_form_path(path_to_search)
        self.old_file_hash = get_file_hash_from_file(file_path)

    def get_changed_list(self):
        changed_list = list()
        for file_path in self.new_file_hash.keys():
            if file_path in self.old_file_hash:
                if self.new_file_hash[file_path] != self.old_file_hash[file_path]:
                    changed_list.append(file_path)
            else:
                changed_list.append(file_path)
        return changed_list

    def save(self, file_name):
        save_file_hash(self.new_file_hash, file_name)

def compile(changes):
    for file_path in changes:
        #if 'src/' in file_path:
        #if source_folder in file_path:
        if '.cpp' in file_path:
            file = file_path.replace('.cpp', '')
        else:
            #print(f"compile: given file doesn't end with .cpp\n\t\tFile Path: {file_path}")
            # file = file_path.replace('.c', '')
            continue
            #file = file_path.replace('.cpp', '')
        file = file.replace(source_folder, '')
        if '/' in file_path:
            file = file.split('/')
            file = file[-1]
        cmd = f"g++ {ret(flags)} {ret(includes)} -c {file_path} -o {o_folder}/{file}.o"
        print(cmd)
        if os.system(cmd) != 0:
            quit(1)

def link():
    o_files = os.listdir(o_folder)
    #src_files = os.listdir(source_folder)
    src_files = [f for f in os.listdir() if os.path.isfile(f)]
    cmd = f'g++ {ret(flags)} {ret(includes)} {ret(libs)} -o {build_folder}/{executable_name} '
    for file in o_files:
        cmd += f'{o_folder}/{file} '
    cmd += ret(link_libs) + " "

    # for file in src_files:
    #     if ".cpp" in file:
    #         cmd += f'{source_folder}/{file} '
    #         #cmd += file + " "

    print(cmd)
    if os.system(cmd) != 0:
        quit(1)


def run():
    print("--- Starting Compilation script ---")
    if not os.path.isfile(saved):
        print(saved + " doesn't exist yet - will be created")
        os.system(f'cd . > {saved}')
    if not os.path.isdir(build_folder):
        print(build_folder + " doesn't exist yet - will be created")
        print(f'mkdir {build_folder}')
        os.system(f'mkdir {build_folder}')
    if not os.path.isdir(o_folder):
        print(o_folder + " doesn't exist yet - will be created")
        os.system(f'mkdir {o_folder}')
    print("creating hashfile")
    hashfile = Builder(search_path, saved)
    print("comparing hashfiles")
    changes = hashfile.get_changed_list()
    print("getting dependecy list")
    dependencies = get_dependency_of_files_from_path(search_path)


    print("checking dependecies")
    for i in range(5): # lol why?
        for changed_file in reversed(changes):
            names = changed_file.split('/')
            file_name = ''
            for i in range(1, len(names)): # why i again?
                name = names[i]
                file_name += name
                file_name += '/'
            file_name = file_name[:-1]
            if file_name in dependencies:
                for dep_file in dependencies[file_name]:
                    if dep_file not in changes:
                        changes.append(dep_file)

    print("starting Compilation")
    compile(changes)
    print("saving hashfile")
    hashfile.save(saved)
    if not changes:
        print("no files need to be recompiled")
    if changes or not os.path.isfile(f'{build_folder}/{executable_name}'):
        link()
        print("done")



run()

