import time
import os
import glob
import xml.etree.ElementTree as ET
import json
import re

#
# Normalize XML tree to single entry array
#
# Useful for fast search in the tree, usually by ID
#
def normalize_xml_tree(treeRoot):
   normalized = [{"tag": treeRoot.tag, "attr": treeRoot.attrib, "obj": treeRoot}]
   for entry in treeRoot:
      normalized = normalized + normalize_xml_tree(entry)
   return normalized

#
# Creates path relative to CMakeLists.txt base path
# And adds (optionally) CMake prefix
#
def gen_relative_path_to_cmake_folder(cmakefolder, path, add_prefix = True, replace_ds = True):
   try:
      path = os.path.relpath(path, cmakefolder)
      if add_prefix:
         path = '${PROJ_PATH}/' + path
   except ValueError:
      print("Value Error triggered")
      print("CmakeFolder:", cmakefolder)
      print("Input path: ", path)
      path = path.replace(cmakefolder, '')
   if replace_ds:
      path = path.replace('\\', '/')
   return path

#
# Main function to parse and generate CMakeLists.txt file
#
def parse_and_generate(projectFolderBasePath):
   print("Configuring project with base path:", projectFolderBasePath)

   # Source files to process
   proj_files = [
      os.path.join(projectFolderBasePath, "STM32CubeIDE/.project"),
      os.path.join(projectFolderBasePath, ".project")
   ]
   cproj_files = [
      os.path.join(projectFolderBasePath, "STM32CubeIDE/.cproject"),
      os.path.join(projectFolderBasePath, ".cproject")
   ]

   # Glob for all source and header files
   data_obj = {
      'project_name': '',
      'configurations': {
         'debug': {
            'target_mcu': '',
            'source_folders': [],
            'fpu': '',
            'float_abi': '',
            'c': {
               'symbols': [],
               'include_paths': [],
               'libraries': [],
               'library_directories': [],
               'debug_level': '',
               'optimization_level': '',
               'other_flags': [],
               'linker_script': '',
            },
            'cxx': {
               'symbols': [],
               'include_paths': [],
               'libraries': [],
               'library_directories': [],
               'debug_level': '',
               'optimization_level': '',
               'other_flags': [],
               'linker_script': '',
            },
            'asm': {
               'symbols': [],
               'include_paths': [],
               'libraries': [],
               'library_directories': [],
               'debug_level': '',
               'optimization_level': '',
               'other_flags': [],
               'linker_script': '',
            },
            'libraries': [],
            'library_directories': [],
            'source_folders': [],
         },
         'release': {
            'target_mcu': '',
            'source_folders': [],
            'fpu': '',
            'float_abi': '',
            'c': {
               'symbols': [],
               'include_paths': [],
               'libraries': [],
               'library_directories': [],
               'debug_level': '',
               'optimization_level': '',
               'other_flags': [],
               'linker_script': '',
            },
            'cxx': {
               'symbols': [],
               'include_paths': [],
               'libraries': [],
               'library_directories': [],
               'debug_level': '',
               'optimization_level': '',
               'other_flags': [],
               'linker_script': '',
            },
            'asm': {
               'symbols': [],
               'include_paths': [],
               'libraries': [],
               'library_directories': [],
               'debug_level': '',
               'optimization_level': '',
               'other_flags': [],
               'linker_script': '',
            },
            'libraries': [],
            'library_directories': [],
            'source_folders': [],
         },
      },
      'linked_files': [],
      'all_source_files_in_path': []
   }

   # Parse XML files
   cproj_parsed = False
   proj_parsed = False
   for f in cproj_files:
      try:
         fTreeCproj = ET.parse(f)

         # Set base folder of .cproject and .project
         CProjBasePath = os.path.dirname(f)

         # Get path difference between where CMakelists.txt file will be placed (project directory)
         # versus where originally .cproject/.project file is located
         CProjBasePath_ProjectBasePath_diff = CProjBasePath.replace(os.path.join(projectFolderBasePath, ''), '')
         cproj_parsed = True
         break
      except:
         pass
   for f in proj_files:
      try:
         fTreeProj = ET.parse(f)
         proj_parsed = True
         break
      except:
         pass
   if not cproj_parsed or not proj_parsed:
      print(".cproject or .project not found or could not be parsed. Exiting the parse process")
      print("--------")
      return

   # Print values to user
   print("Project top folder path:                  ", projectFolderBasePath)
   print("Project .cproject and .project path:      ", CProjBasePath)
   print("Project base vs .c/.cproj difference path:", CProjBasePath_ProjectBasePath_diff)

   #
   # We can normalize tree to one array once we are inside one configuration.
   # Every feature has a "superClass" tag, that can be used for identification purpose
   #
   fTreeRootCproj = fTreeCproj.getroot()
   fTreeCprojNormalized = normalize_xml_tree(fTreeRootCproj)
   for tEntry in fTreeCprojNormalized:
      #
      # We want to parse debug configuration only for the moment
      #
      print(tEntry['tag'])
      if tEntry['tag'] == 'cconfiguration' and 'id' in tEntry['attr'] and 'com.st.stm32cube.ide.mcu.gnu.managedbuild.config.exe.debug' in tEntry['attr']['id']:
         conf_key = 'debug'

         # Get all entries, normalized for specific configuration mode
         all_sub_entries = normalize_xml_tree(tEntry['obj'])
         for entry in all_sub_entries:
            if 'superClass' in entry['attr']:
               # Get target MCU
               if entry['attr']['superClass'] == 'com.st.stm32cube.ide.mcu.gnu.managedbuild.option.target_mcu' and 'value' in entry['attr']:
                  data_obj['configurations'][conf_key]['target_mcu'] = entry['attr']['value']

               # FPU settings
               if entry['attr']['superClass'] == 'com.st.stm32cube.ide.mcu.gnu.managedbuild.option.floatabi' and 'value' in entry['attr']:
                  data_obj['configurations'][conf_key]['float_abi'] = entry['attr']['value'].replace('com.st.stm32cube.ide.mcu.gnu.managedbuild.option.floatabi.value.', '')
               if entry['attr']['superClass'] == 'com.st.stm32cube.ide.mcu.gnu.managedbuild.option.fpu' and 'value' in entry['attr']:
                  data_obj['configurations'][conf_key]['fpu'] = entry['attr']['value'].replace('com.st.stm32cube.ide.mcu.gnu.managedbuild.option.fpu.value.', '')

               #
               # MCU GCC Assembler options
               #
               if entry['tag'] == 'tool' and entry['attr']['superClass'] == 'com.st.stm32cube.ide.mcu.gnu.managedbuild.tool.assembler':
                  continue
               if entry['tag'] == 'option' and entry['attr']['superClass'] == 'com.st.stm32cube.ide.mcu.gnu.managedbuild.tool.assembler.compiler':
                  continue
               if entry['tag'] == 'option' and entry['attr']['superClass'] == 'com.st.stm32cube.ide.mcu.gnu.managedbuild.tool.assembler.option.definedsymbols':
                  # List all symbols for compiler
                  for listOptionValue in entry["obj"]:
                     data_obj['configurations'][conf_key]['asm']['symbols'].append(listOptionValue.attrib['value'])
                  continue
               if entry['tag'] == 'option' and entry['attr']['superClass'] == 'com.st.stm32cube.ide.mcu.gnu.managedbuild.tool.assembler.option.includepaths':
                  # List all include paths for compiler
                  for listOptionValue in entry["obj"]:
                     data_obj['configurations'][conf_key]['asm']['include_paths'].append(listOptionValue.attrib['value'])
                  continue
               if entry['tag'] == 'option' and entry['attr']['superClass'] == 'com.st.stm32cube.ide.mcu.gnu.managedbuild.tool.assembler.option.debuglevel' and 'value' in entry['attr']:
                  data_obj['configurations'][conf_key]['asm']['debug_level'] = entry['attr']['value'].replace('com.st.stm32cube.ide.mcu.gnu.managedbuild.tool.assembler.option.debuglevel.value.', '')
                  continue
               if entry['tag'] == 'option' and entry['attr']['superClass'] == 'com.st.stm32cube.ide.mcu.gnu.managedbuild.tool.assembler.option.optimization.level' and 'value' in entry['attr']:
                  data_obj['configurations'][conf_key]['asm']['optimization_level'] = entry['attr']['value'].replace('com.st.stm32cube.ide.mcu.gnu.managedbuild.tool.assembler.option.optimization.level.value.', '')
                  continue
               if entry['tag'] == 'option' and entry['attr']['superClass'] == 'com.st.stm32cube.ide.mcu.gnu.managedbuild.tool.assembler.option.otherflags':
                  for listOptionValue in entry["obj"]:
                     data_obj['configurations'][conf_key]['asm']['other_flags'].append(listOptionValue.attrib['value'])
                  continue

               #
               # MCU C Compiler options
               #
               if entry['tag'] == 'tool' and entry['attr']['superClass'] == 'com.st.stm32cube.ide.mcu.gnu.managedbuild.tool.c.compiler':
                  continue
               if entry['tag'] == 'option' and entry['attr']['superClass'] == 'com.st.stm32cube.ide.mcu.gnu.managedbuild.tool.c.compiler.option.definedsymbols':
                  # List all symbols for compiler
                  for listOptionValue in entry["obj"]:
                     data_obj['configurations'][conf_key]['c']['symbols'].append(listOptionValue.attrib['value'])
                  continue
               if entry['tag'] == 'option' and entry['attr']['superClass'] == 'com.st.stm32cube.ide.mcu.gnu.managedbuild.tool.c.compiler.option.includepaths':
                  # List all include paths for compiler
                  for listOptionValue in entry["obj"]:
                     data_obj['configurations'][conf_key]['c']['include_paths'].append(listOptionValue.attrib['value'])
                  continue
               if entry['tag'] == 'option' and entry['attr']['superClass'] == 'com.st.stm32cube.ide.mcu.gnu.managedbuild.tool.c.compiler.option.debuglevel':
                  data_obj['configurations'][conf_key]['c']['debug_level'] = entry['attr']['value'].replace('com.st.stm32cube.ide.mcu.gnu.managedbuild.tool.c.compiler.option.debuglevel.value.', '')
                  continue
               if entry['tag'] == 'option' and entry['attr']['superClass'] == 'com.st.stm32cube.ide.mcu.gnu.managedbuild.tool.c.compiler.option.optimization.level' and 'value' in entry['attr']:
                  data_obj['configurations'][conf_key]['c']['optimization_level'] = entry['attr']['value'].replace('com.st.stm32cube.ide.mcu.gnu.managedbuild.tool.c.compiler.option.optimization.level.value.', '')
                  continue
               if entry['tag'] == 'option' and entry['attr']['superClass'] == 'com.st.stm32cube.ide.mcu.gnu.managedbuild.tool.c.compiler.option.otherflags' and 'value' in entry['attr']:
                  for listOptionValue in entry["obj"]:
                     data_obj['configurations'][conf_key]['c']['other_flags'].append(listOptionValue.attrib['value'])
                  continue

               #
               # MCU CXX Compiler options
               #
               if entry['tag'] == 'tool' and entry['attr']['superClass'] == 'com.st.stm32cube.ide.mcu.gnu.managedbuild.tool.cpp.compiler':
                  continue
               if entry['tag'] == 'option' and entry['attr']['superClass'] == 'com.st.stm32cube.ide.mcu.gnu.managedbuild.tool.cpp.compiler.option.definedsymbols':
                  # List all symbols for compiler
                  for listOptionValue in entry["obj"]:
                     data_obj['configurations'][conf_key]['cxx']['symbols'].append(listOptionValue.attrib['value'])
                  continue
               if entry['tag'] == 'option' and entry['attr']['superClass'] == 'com.st.stm32cube.ide.mcu.gnu.managedbuild.tool.cpp.compiler.option.includepaths':
                  # List all include paths for compiler
                  for listOptionValue in entry["obj"]:
                     data_obj['configurations'][conf_key]['cxx']['include_paths'].append(listOptionValue.attrib['value'])
                  continue
               if entry['tag'] == 'tool' and entry['attr']['superClass'] == 'com.st.stm32cube.ide.mcu.gnu.managedbuild.tool.cpp.linker':
                  continue
               if entry['tag'] == 'option' and entry['attr']['superClass'] == 'com.st.stm32cube.ide.mcu.gnu.managedbuild.tool.cpp.compiler.option.debuglevel' and 'value' in entry['attr']:
                  data_obj['configurations'][conf_key]['cxx']['debug_level'] = entry['attr']['value'].replace('com.st.stm32cube.ide.mcu.gnu.managedbuild.tool.cpp.compiler.option.debuglevel.value.', '')
                  continue
               if entry['tag'] == 'option' and entry['attr']['superClass'] == 'com.st.stm32cube.ide.mcu.gnu.managedbuild.tool.cpp.compiler.option.optimization.level' and 'value' in entry['attr']:
                  data_obj['configurations'][conf_key]['cxx']['optimization_level'] = entry['attr']['value'].replace('com.st.stm32cube.ide.mcu.gnu.managedbuild.tool.cpp.compiler.option.optimization.level.value.', '')
                  continue
               if entry['tag'] == 'option' and entry['attr']['superClass'] == 'com.st.stm32cube.ide.mcu.gnu.managedbuild.tool.cpp.compiler.option.otherflags':
                  for listOptionValue in entry["obj"]:
                     data_obj['configurations'][conf_key]['cxx']['other_flags'].append(listOptionValue.attrib['value'])
                  continue

               #
               # MCU GCC Linker
               #
               if entry['tag'] == 'option' and entry['attr']['superClass'] == 'com.st.stm32cube.ide.mcu.gnu.managedbuild.tool.c.linker.option.script' and 'value' in entry['attr']:
                  data_obj['configurations'][conf_key]['c']['linker_script'] = entry['attr']['value']
                  continue
               if entry['tag'] == 'option' and entry['attr']['superClass'] == 'com.st.stm32cube.ide.mcu.gnu.managedbuild.tool.c.linker.option.libraries':
                  # List all external libraries for CXX compiler
                  for listOptionValue in entry["obj"]:
                     data_obj['configurations'][conf_key]['c']['libraries'].append(listOptionValue.attrib['value'])
                     data_obj['configurations'][conf_key]['libraries'].append(listOptionValue.attrib['value'])
                  continue
               if entry['tag'] == 'option' and entry['attr']['superClass'] == 'com.st.stm32cube.ide.mcu.gnu.managedbuild.tool.c.linker.option.directories':
                  # List all external library directories
                  for listOptionValue in entry["obj"]:
                     data_obj['configurations'][conf_key]['c']['library_directories'].append(listOptionValue.attrib['value'])
                     data_obj['configurations'][conf_key]['library_directories'].append(listOptionValue.attrib['value'])
                  continue

               #
               # MCU GXX Linker
               #
               if entry['tag'] == 'option' and entry['attr']['superClass'] == 'com.st.stm32cube.ide.mcu.gnu.managedbuild.tool.cpp.linker.option.script' and 'value' in entry['attr']:
                  data_obj['configurations'][conf_key]['cxx']['linker_script'] = entry['attr']['value']
                  continue
               if entry['tag'] == 'option' and entry['attr']['superClass'] == 'com.st.stm32cube.ide.mcu.gnu.managedbuild.tool.cpp.linker.option.libraries':
                  # List all external libraries for CXX compiler
                  for listOptionValue in entry["obj"]:
                     data_obj['configurations'][conf_key]['cxx']['libraries'].append(listOptionValue.attrib['value'])
                     data_obj['configurations'][conf_key]['libraries'].append(listOptionValue.attrib['value'])
                  continue
               if entry['tag'] == 'option' and entry['attr']['superClass'] == 'com.st.stm32cube.ide.mcu.gnu.managedbuild.tool.cpp.linker.option.directories':
                  # List all external library directories
                  for listOptionValue in entry["obj"]:
                     data_obj['configurations'][conf_key]['cxx']['library_directories'].append(listOptionValue.attrib['value'])
                     data_obj['configurations'][conf_key]['library_directories'].append(listOptionValue.attrib['value'])
                  continue

            # Check for source folders
            # Very experimental -> needs stronger checks
            elif entry['tag'] == "sourceEntries":
               for folderEntry in entry['obj']:
                  if 'name' in folderEntry.attrib and folderEntry.attrib['name'] != '':
                     name = folderEntry.attrib['name']
                     files = []
                     files = files + glob.glob(os.path.join(CProjBasePath, name, '**/*.c'), recursive = True)
                     files = files + glob.glob(os.path.join(CProjBasePath, name, '**/*.cpp'), recursive = True)
                     files = files + glob.glob(os.path.join(CProjBasePath, name, '**/*.s'), recursive = True)
                     data_obj['configurations'][conf_key]['source_folders'].append({'name': name, 'files': files})

         # Stop executing after first "debug" config
         break

   #
   # Glob for files in the same directory as .project and .cproject
   #
   # These are automatically added to the project
   #
   files_scan = []
   files_scan = files_scan + glob.glob(os.path.join(CProjBasePath, "**/*.c"), recursive = True)
   files_scan = files_scan + glob.glob(os.path.join(CProjBasePath, "**/*.cpp"), recursive = True)
   files_scan = files_scan + glob.glob(os.path.join(CProjBasePath, "**/*.s"), recursive = True)
   files_scan = [os.path.join(CProjBasePath, f) for f in files_scan]

   # Remove files from build directory
   files = []
   for file in files_scan:
      if os.path.join(projectFolderBasePath, 'build') in file:
         continue
      add_to_list = True
      for conf in ['debug']:
         for srcGrp in data_obj['configurations'][conf]['source_folders']:
            for f in srcGrp['files']:
               if file == f:
                  add_to_list = False
      if add_to_list:
         files.append(file)

   # Add full path to the files indeed
   data_obj['all_source_files_in_path'] = files

   #
   # Handle .project file
   #
   fTreeRootProj = fTreeProj.getroot()
   fTreeRootProjNormalized = normalize_xml_tree(fTreeRootProj)
   for treeEntry in fTreeRootProjNormalized:
      # Check project description
      if treeEntry['tag'] == 'projectDescription':
         for descEntry in treeEntry['obj']:
            if descEntry.tag == 'name':
               data_obj['project_name'] = descEntry.text

      # Check for linked resources
      if treeEntry['tag'] == 'linkedResources':
         # List all links in the chain
         for linkEntry in treeEntry['obj']:
            path = ''
            for linkEntryTag in linkEntry:
               if linkEntryTag.tag == 'locationURI':
                  path = linkEntryTag.text.replace('$%7B', '').replace('%7D', '').replace('$%7', '')
                  prog = re.compile('PARENT-([0-9]+)-PROJECT_LOC')
                  result = prog.search(path)
                  if result:
                     # Add "folder-up" prefix number of times, linked to matched number
                     # Then replace "PARENT-xxx-PROJECT_LOC/" with white-space
                     path = ('../' * int(result.group(1))) + path.replace(result.group(0) + '/', '')

                  # Create full path
                  path = os.path.normpath(os.path.join(CProjBasePath, path))
            if path != '':
               data_obj['linked_files'].append(path)

   print(json.dumps(data_obj, indent = 4))

   ##
   # TODO: Ignore any .c file from "/build/" directory
   # This is ninja build system
   #

   # Read template file
   templatefiledata = ''
   with open("templates/CMakeLists_template.txt", "r") as file:
      templatefiledata = file.read()

   # Set project name
   project_name = data_obj['project_name']
   if len(project_name) == 0:
      project_name = 'unknown'
   templatefiledata = templatefiledata.replace('{{sr:project_name}}', project_name)

   # Set MCU specifics
   for conf in ['debug']:
      # Set Cortex
      target_mcu = data_obj['configurations'][conf]['target_mcu'][0:7].upper()
      if len(target_mcu) < 7:
         print("Target_MCU len is less than 7 characters. Not enough to determine STM32 Cortex-M CPU")
         continue
      target_mcu = target_mcu[0:7].upper()
      if target_mcu in ['STM32F0']:
         target_cpu = '-mcpu=cortex-m0'
      elif target_mcu in ['STM32L0', 'STM32G0']:
         target_cpu = '-mcpu=cortex-m0plus'
      elif target_mcu in ['STM32F1', 'STM32F2', 'STM32L1']:
         target_cpu = '-mcpu=cortex-m3'
      elif target_mcu in ['STM32F3', 'STM32F4', 'STM32L4', 'STM32G4', 'STM32WB', 'STM32WL']:
         target_cpu = '-mcpu=cortex-m4'
      elif target_mcu in ['STM32L5', 'STM32U5', 'STM32H5']:
         target_cpu = '-mcpu=cortex-m33'
      elif target_mcu in ['STM32F7', 'STM32H7']:
         target_cpu = '-mcpu=cortex-m7'
      else:
         printf("Unknown STM32")
         return

      # Set floating point
      target_fpu = data_obj['configurations'][conf]['fpu']
      if len(target_fpu) > 0:
         target_fpu = '-mfpu=' + target_fpu

      # Set floating point API
      target_fpu_abi = data_obj['configurations'][conf]['float_abi']
      if len(target_fpu_abi) > 0:
         target_fpu_abi = '-mfloat-abi=' + target_fpu_abi

      # Make replacements
      templatefiledata = templatefiledata.replace('{{sr:m_cpu}}', target_cpu)
      templatefiledata = templatefiledata.replace('{{sr:m_fpu}}', target_fpu)
      templatefiledata = templatefiledata.replace('{{sr:m_float_abi}}', target_fpu_abi)

   # Check all linked files from .cproject file
   templatefiledata = templatefiledata.replace(
      '{{sr:linked_SRCS}}',
      '\n\t'.join([gen_relative_path_to_cmake_folder(projectFolderBasePath, p) for p in data_obj['linked_files']])
   )

   # Check all files in the same directory as .cproject/.project directory
   templatefiledata = templatefiledata.replace(
      '{{sr:all_project_dir_SRCS}}',
      '\n\t'.join([gen_relative_path_to_cmake_folder(projectFolderBasePath, p) for p in data_obj['all_source_files_in_path']])
   )

   #
   # Check source folders and respective files, defined in .project file
   # Group variables by source folder name
   #
   for conf in ['debug']:
      cmake_set_arr = []               # List of "set" commands
      cmake_source_vars_arr = []       # List of "set" variable names
      for sourceGroup in data_obj['configurations'][conf]['source_folders']:
         srcGroupFilesVarName = 'source_folder_' + sourceGroup['name'] + '_SRCS'
         srcGroupFilesText = '\n\t' + '\n\t'.join([gen_relative_path_to_cmake_folder(projectFolderBasePath, f) for f in sourceGroup['files']])
         cmake_set_arr.append('set(' + srcGroupFilesVarName + ' ' + srcGroupFilesText + ')')
         cmake_source_vars_arr.append('${' + srcGroupFilesVarName + '}')

      # Replace file with text
      templatefiledata = templatefiledata.replace('{{sr:set_source_folder_files}}', '\n\n'.join(cmake_set_arr))
      templatefiledata = templatefiledata.replace('{{sr:set_source_folder_files_variables}}', '\n\t' + '\n\t'.join(cmake_source_vars_arr))

   #
   # Check include paths
   # Split between each of the compiler types
   #
   for conf in ['debug']:
      for compiler in ['c', 'cxx', 'asm']:
         varname = 'include_' + compiler + '_DIRS'
         paths = []
         for path in data_obj['configurations'][conf][compiler]['include_paths']:
            # We are here adding "Debug" fake name
            # because "cwd" path for CubeIDE is "<location_of_.cproject>/Debug"
            path = os.path.join(os.path.join(CProjBasePath, 'Debug'), path)
            # Normalize path to remove "Debug" from path
            paths.append(os.path.normpath(path))
            print(os.path.normpath(path))
         templatefiledata = templatefiledata.replace('{{sr:' + varname + '}}', '\n\t'.join([gen_relative_path_to_cmake_folder(projectFolderBasePath, p) for p in paths]))

   #
   # Check all symbols (global defines)
   # Split between each of the compiler types
   #
   for conf in ['debug']:
      for compiler in ['c', 'cxx', 'asm']:
         varname = 'symbols_' + compiler + '_SYMB'
         templatefiledata = templatefiledata.replace('{{sr:' + varname + '}}', "\n\t".join(["\"" + f + "\"" for f in data_obj['configurations'][conf][compiler]['symbols']]))

   #
   # Setup linked libraries
   #
   for conf in ['debug']:
      libs = data_obj['configurations'][conf]['libraries']
      paths = data_obj['configurations'][conf]['library_directories']
      for i in range(len(paths)):
         # Do some optimizations with path if necessary
         pass
      templatefiledata = templatefiledata.replace('{{sr:link_DIRS}}', "\n\t".join([gen_relative_path_to_cmake_folder(projectFolderBasePath, os.path.normpath(os.path.join(CProjBasePath, 'Debug', p))) for p in paths]))
      templatefiledata = templatefiledata.replace('{{sr:link_LIBS}}', "\n\t".join(libs))

   #
   # Setup linker script
   #
   for conf in ['debug']:
      var_replace_name = '{{sr:linker_script_SRC}}'
      linker_file_orig = data_obj['configurations'][conf]['c']['linker_script']
      linker_file = linker_file_orig.replace('${workspace_loc:/${ProjName}/', '')
      linker_file = linker_file.replace('workspace_loc:', '').replace('', '')
      linker_file = linker_file.replace('${ProjName}', '')
      linker_file = linker_file.replace('}', '')
      linker_file = os.path.join(CProjBasePath, linker_file)
      # Text format: '${workspace_loc:/${ProjName}/STM32G474RETX_FLASH.ld}'
      templatefiledata = templatefiledata.replace(var_replace_name, gen_relative_path_to_cmake_folder(projectFolderBasePath, linker_file))

   # Print-out JSON dump
   #print(json.dumps(data_obj, indent = 4))

   # Write data to file
   cmakelistsfile = os.path.join(projectFolderBasePath, 'CMakeLists.txt')
   with open(cmakelistsfile, "w") as file:
      file.write(templatefiledata)

   # Copy compiler .cmake file to user path
   dotCmakeCompilerTargetFile = os.path.join(projectFolderBasePath, 'gcc-arm-none-eabi.cmake')
   data = ''
   with open('templates/gcc-arm-none-eabi.cmake', 'r') as file:
      data = file.read()
   with open(dotCmakeCompilerTargetFile, "w") as file:
      file.write(data)

   # That's it
   print("CMakeLists.txt file generated:", cmakelistsfile)
   print("Finished")
   print("--------")

##########################################################
# Run script
##########################################################

# Get all projects in this folder
basepath = os.path.dirname(os.path.abspath(__file__))
allProjects = glob.glob(basepath + "/script-projects/*/")
#allProjects = [
#   os.path.join(basepath, 'h735g-dk-usart')
#]
for projectFolder in allProjects:
   parse_and_generate(projectFolder)