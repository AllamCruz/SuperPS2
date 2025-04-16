package com.superps2.emu

import android.app.Activity
import android.content.Intent
import android.net.Uri
import android.os.Build
import android.os.Bundle
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.GravityCompat
import androidx.drawerlayout.widget.DrawerLayout
import com.google.android.material.floatingactionbutton.FloatingActionButton
import com.google.android.material.navigation.NavigationView

class MainActivity : AppCompatActivity() {

    companion object {
        // Carrega a biblioteca nativa ao iniciar a classe
        init {
            System.loadLibrary("native-lib")
        }
        private const val REQUEST_CODE_OPEN_DIRECTORY = 1001
    }

    // Declaração da função nativa via JNI para inicializar o emulador
    external fun initEmulator(): Boolean

    // Declaração dos componentes da interface
    private lateinit var tvDeviceInfo: TextView
    private lateinit var fabAddISO: FloatingActionButton
    private lateinit var drawerLayout: DrawerLayout
    private lateinit var navView: NavigationView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        // Define o layout da Activity conforme o arquivo activity_main.xml
        setContentView(R.layout.activity_main)

        // Inicializa os elementos da interface utilizando os IDs definidos no XML
        tvDeviceInfo = findViewById(R.id.tvDeviceInfo)
        fabAddISO = findViewById(R.id.fabAddISO)
        drawerLayout = findViewById(R.id.drawer_layout)
        navView = findViewById(R.id.navView)

        // Configura as informações do dispositivo (nome, chipset, memória, % compatibilidade)
        setupDeviceInfo()

        // Configura o Floating Action Button para selecionar a pasta dos ISOs
        setupFloatingActionButton()

        // Configura o Navigation Drawer com as opções disponíveis
        setupNavigationDrawer()

        // Inicializa a camada nativa do emulador via JNI
        val emulatorInitialized = initEmulator()
        if (!emulatorInitialized) {
            // Tratar a falha na inicialização do emulador, se necessário.
        }

        // FUTURAS INTEGRAÇÕES:
        // Aqui podem ser adicionados lógicas de renderização, controle de input dos jogos e
        // outras configurações avançadas, como otimizações para processadores Snapdragon.
    }

    /**
     * Recupera e exibe as informações do dispositivo utilizando APIs do Android.
     * Este método obtém informações básicas como modelo, chipset, memória RAM e um valor
     * de compatibilidade (inicialmente placeholder) que podem ser atualizados futuramente.
     */
    private fun setupDeviceInfo() {
        // Obtem o nome do dispositivo a partir de Build (ex.: "SM-G998B", "Pixel 6")
        val deviceName = Build.MODEL
        // Obtem a referência de hardware, que pode indicar o chipset (ex.: "exynos2100")
        val chipset = Build.HARDWARE
        // Obtém a informação da memória RAM. A função getRamInfo() pode ser expandida para lógica real.
        val ramInfo = getRamInfo()
        // Placeholder para % de compatibilidade, a ser atualizado conforme a lógica do emulador
        val compatibilityPercent = "100%"

        // Concatena as informações em uma única String para exibição
        val deviceInfoText = "Dispositivo: $deviceName | Chipset: $chipset | RAM: $ramInfo | Compatibilidade: $compatibilityPercent"
        tvDeviceInfo.text = deviceInfoText
    }

    /**
     * Método auxiliar para obter a informação da memória RAM do dispositivo.
     * Esta função é um placeholder e pode ser aprimorada para retornar dados reais.
     */
    private fun getRamInfo(): String {
        // Lógica real pode utilizar ActivityManager e MemoryInfo para obter dados precisos
        return "4GB"  // Valor fixo de exemplo
    }

    /**
     * Configura o Floating Action Button (FAB) que, ao ser clicado, abre o seletor
     * de pastas utilizando o Storage Access Framework, permitindo a adição dos arquivos ISO.
     */
    private fun setupFloatingActionButton() {
        fabAddISO.setOnClickListener {
            // Cria um Intent para selecionar um diretório (ACTION_OPEN_DOCUMENT_TREE)
            val intent = Intent(Intent.ACTION_OPEN_DOCUMENT_TREE).apply {
                // Permite a leitura e escrita e a persistência das permissões
                addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION or Intent.FLAG_GRANT_WRITE_URI_PERMISSION)
                addFlags(Intent.FLAG_GRANT_PERSISTABLE_URI_PERMISSION)
            }
            // Inicia a Activity para seleção de diretório e espera um resultado
            startActivityForResult(intent, REQUEST_CODE_OPEN_DIRECTORY)
        }
    }

    /**
     * Configura o Navigation Drawer (menu lateral) e define as ações para cada item.
     * As opções incluem "Página Principal", "Configurações" e "Idioma".
     */
    private fun setupNavigationDrawer() {
        navView.setNavigationItemSelectedListener { menuItem ->
            when (menuItem.itemId) {
                R.id.nav_home -> {
                    // Ação para "Página Principal"
                    // Aqui pode ser implementada a lógica para exibir a página principal
                    drawerLayout.closeDrawer(GravityCompat.START)
                    true
                }
                R.id.nav_settings -> {
                    // Ação para "Configurações"
                    // Exemplo: iniciar a Activity de configurações
                    // startActivity(Intent(this, SettingsActivity::class.java))
                    drawerLayout.closeDrawer(GravityCompat.START)
                    true
                }
                R.id.nav_language -> {
                    // Ação para trocar o idioma
                    // Exemplo: abrir um diálogo para seleção de idioma ou atualizar as configurações do app
                    drawerLayout.closeDrawer(GravityCompat.START)
                    true
                }
                else -> false
            }
        }
    }

    /**
     * Método para tratar o retorno do seletor de diretórios.
     * Utiliza o método takePersistableUriPermission para garantir o acesso contínuo ao diretório selecionado.
     */
    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (requestCode == REQUEST_CODE_OPEN_DIRECTORY && resultCode == Activity.RESULT_OK) {
            data?.data?.let { uri ->
                // Persiste a permissão de leitura e escrita para o diretório selecionado
                contentResolver.takePersistableUriPermission(
                    uri,
                    Intent.FLAG_GRANT_READ_URI_PERMISSION or Intent.FLAG_GRANT_WRITE_URI_PERMISSION
                )
                // A partir deste ponto, o caminho selecionado pode ser utilizado para acessar os arquivos ISO
            }
        }
    }
}
