# Módulo core do emulador PS2

# Verificar se o target existe
if(NOT TARGET superps2)
    message(FATAL_ERROR "Target 'superps2' não encontrado")
endif()

# Obter todos os arquivos .cpp do diretório
file(GLOB CORE_SOURCES "*.cpp")

# Verificar se existem fontes
if(NOT CORE_SOURCES)
    message(WARNING "Nenhum arquivo fonte encontrado em ${CMAKE_CURRENT_SOURCE_DIR}")
else()
    message(STATUS "Fontes encontradas em core: ${CORE_SOURCES}")
endif()

# Adicionar fontes à biblioteca principal
target_sources(superps2 PRIVATE ${CORE_SOURCES})

# Adicionar diretório de includes
target_include_directories(superps2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}) 