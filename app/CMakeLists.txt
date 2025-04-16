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

# Criação de uma biblioteca compartilhada (shared library) que será utilizada pelo aplicativo Android.
# A biblioteca é denominada "native-lib".
add_library(
    native-lib                 # Nome da biblioteca
    SHARED                     # Tipo: biblioteca compartilhada
    ${SRC_DIR}/native-lib.cpp  # Arquivo fonte da biblioteca
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
add_library(superps2 SHARED
    src/main/cpp/native-lib.cpp
    src/main/cpp/ps2engine/ps2engine.cpp
    src/main/cpp/renderer/ps2renderer.cpp
)

# Inclui o novo módulo ps2engine
target_sources(superps2 PRIVATE
    src/main/cpp/ps2engine/ps2engine.cpp
    src/main/cpp/emulation/ps2core.cpp
    src/main/cpp/emulation/bios/bios.cpp
    src/main/cpp/emulation/memory/memory.cpp
    src/main/cpp/emulation/cpu/ee_core.cpp
)
include_directories(src/main/cpp/ps2engine)
include_directories(src/main/cpp/emulation/bios)
# FUTURAS EXPANSÕES:
#
# Para adicionar outros arquivos fonte ou bibliotecas nativas, basta incluir as diretivas
# apropriadas aqui. Por exemplo, se futuramente for necessário compilar módulos adicionais
# para o emulador, adicione-os à lista de fontes ou inclua novas chamadas de target_link_libraries.
#
# Ajustes adicionais podem ser realizados modificando as flags de compilação ou adicionando
# definições específicas para processadores Snapdragon, como flags para otimizações com NEON.
