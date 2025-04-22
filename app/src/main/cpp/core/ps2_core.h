#ifndef SUPERPS2_PS2_CORE_H
#define SUPERPS2_PS2_CORE_H

#include <string>
#include <memory>
#include <thread>
#include <atomic>

namespace ps2 {

    /**
     * Inicializa o núcleo do emulador com o caminho da ISO.
     * @param isoPath Caminho absoluto da ISO.
     * @return true se a inicialização for bem-sucedida.
     */
    bool initializeCore(const std::string& isoPath);

    /**
     * Executa o ciclo principal da emulação.
     * Deve ser chamado por uma thread nativa.
     */
    void run();

    /**
     * Pausa a execução da emulação.
     */
    void pause();

    /**
     * Retoma a execução da emulação.
     */
    void resume();

    /**
     * Reseta o estado da emulação.
     */
    void reset();

    /**
     * Salva o estado atual da emulação em um arquivo.
     * @param savePath Caminho para salvar o estado.
     * @return true se o salvamento for bem-sucedido.
     */
    bool saveStateToFile(const std::string& savePath);

    /**
     * Carrega um estado salvo da emulação de um arquivo.
     * @param savePath Caminho do estado a ser carregado.
     * @return true se o carregamento for bem-sucedido.
     */
    bool loadStateFromFile(const std::string& savePath);

    /**
     * Finaliza e limpa os recursos do núcleo.
     */
    void shutdown();

    /**
     * Verifica se o núcleo está em execução.
     * @return true se o núcleo estiver rodando.
     */
    bool isRunning();

    /**
     * Verifica se o núcleo está pausado.
     * @return true se o núcleo estiver pausado.
     */
    bool isPaused();

    /**
     * Inicia a thread de emulação.
     */
    void launchEmulationThread();

    /**
     * Para a thread de emulação.
     */
    void stopEmulationThread();
}

#endif //SUPERPS2_PS2_CORE_H 