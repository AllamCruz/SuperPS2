# Módulo de emulação do emulador PS2

# Obter todos os arquivos .cpp do diretório
file(GLOB EMULATION_SOURCES "*.cpp")

# Adicionar fontes à biblioteca principal
target_sources(superps2 PRIVATE ${EMULATION_SOURCES})

# Adicionar diretório de includes
target_include_directories(superps2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}) 