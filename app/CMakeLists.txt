# Versão mínima do CMake requerida para o projeto. 
# Utilizamos a versão 3.10 para garantir compatibilidade com recursos modernos.
cmake_minimum_required(VERSION 3.10)

# Define o nome do projeto
project(SuperPS2)

# Configuração do padrão C++: definições para utilizar C++17.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Flags de compilação: -O2 para otimização de desempenho.
# Estes flags podem ser ajustados para níveis superiores conforme necessário.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

# Diretório onde se encontram os arquivos fonte nativos.
# Neste exemplo, o arquivo native-lib.cpp está localizado em "app/src/main/cpp/"
set(SRC_DIR "${CMAKE_SOURCE_DIR}/app/src/main/cpp")

# Adiciona os arquivos do core
file(GLOB_RECURSE CORE_SRC
    "src/main/cpp/core/*.cpp"
)

# Criação de uma biblioteca compartilhada (shared library) que será utilizada pelo aplicativo Android.
# A biblioteca é denominada "native-lib".
add_library(
    native-lib                 # Nome da biblioteca
    SHARED                     # Tipo: biblioteca compartilhada
    ${SRC_DIR}/native-lib.cpp  # Arquivo fonte da biblioteca
    ${CORE_SRC}
)

# Localiza a biblioteca "log" do Android, necessária para o sistema de logging.
# A função find_library garante que a biblioteca seja corretamente linkada no build.
find_library(
    log-lib     # Nome da variável que armazenará o caminho da biblioteca
    log         # Nome da biblioteca (log do Android)
)

# Linka a biblioteca nativa com as bibliotecas do sistema (neste caso, a biblioteca "log").
# Outras bibliotecas podem ser adicionadas a esta seção conforme o projeto evolua.
target_link_libraries(
    native-lib  # Biblioteca a ser linkada
    ${log-lib}  # Bibliotecas externas a serem utilizadas (Android log)
)

# Inclui o novo módulo ps2engine
target_sources(superps2 PRIVATE
    src/main/cpp/ps2engine/ps2engine.cpp
    src/main/cpp/emulation/ps2core.cpp
    src/main/cpp/emulation/bios/bios.cpp
    src/main/cpp/emulation/memory/memory.cpp
    src/main/cpp/emulation/cpu/ee_core.cpp
    src/main/cpp/emulation/dma/dma.cpp
    src/main/cpp/emulation/vif/vif.cpp
    src/main/cpp/emulation/gif/gif.cpp
    src/main/cpp/emulation/state/state.cpp
    src/main/cpp/emulation/memorycard/memorycard.cpp
    src/main/cpp/emulation/cheats/cheat.cpp
    src/main/cpp/jni/StateBridge.cpp
    src/main/cpp/osd/osd.cpp
    src/main/cpp/emulation/options/engine_options.cpp
    src/main/cpp/emulation/vu0/vu0.cpp
    src/main/cpp/emulation/vu1/vu1.cpp
    src/main/cpp/emulation/scratchpad/scratchpad.cpp
    src/main/cpp/emulation/iop/iop.cpp
    src/main/cpp/emulation/ee/interrupt.cpp
    src/main/cpp/emulation/spu2/spu2.cpp
    src/main/cpp/emulation/ee/memory/ram.cpp
    src/main/cpp/emulation/ee/memory/mem_mainram.cpp
    src/main/cpp/emulation/ee/memory/memory.cpp
    src/main/cpp/emulation/ee/cop0/cop0.cpp
    src/main/cpp/emulation/ee/cpu/ee_core.cpp
    src/main/cpp/emulation/ee/vu/vu0.cpp
    src/main/cpp/emulation/ee/vu/vu1.cpp
    src/main/cpp/emulation/ee/cpu/gpr.cpp
    src/main/cpp/emulation/ee/cpu/pc.cpp
    src/main/cpp/emulation/ee/cpu/decoder.cpp
    src/main/cpp/emulation/ee/cpu/interpreter.cpp
    src/main/cpp/jni/ps2_jni_bridge.cpp
    src/main/cpp/renderer/renderer.cpp
    src/main/cpp/jni/input_bridge.cpp
    src/main/cpp/jni/emulator_bridge.cpp
    src/main/cpp/emulation/input/input_manager.cpp
    src/main/cpp/graphics/gl_renderer.cpp
)
include_directories(src/main/cpp/ps2engine)
include_directories(src/main/cpp/emulation/bios)

# Integração do subsistema de áudio SPU2
target_sources(superps2 PRIVATE
    src/main/cpp/audio/spu2.cpp
    src/main/cpp/jni/native-audio-bridge.cpp
)

# Adiciona zlib para cálculo de CRC32
find_package(ZLIB REQUIRED)
target_link_libraries(superps2 PRIVATE ${log-lib} ${aaudio-lib} ZLIB::ZLIB)

target_link_libraries(superps2 PRIVATE
    android
    log
    EGL
    GLESv2
)

# Find and link against Oboe
find_library(oboe-lib oboe)
target_link_libraries(superps2 PRIVATE
    android
    log
    EGL
    GLESv2
    ${oboe-lib}
)