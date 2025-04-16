package com.superps2.emu.ui

import android.app.ActivityManager
import android.content.Context
import android.os.Build
import android.os.Bundle
import android.widget.TextView
import androidx.appcompat.app.ActionBarDrawerToggle
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.GravityCompat
import androidx.navigation.findNavController
import androidx.navigation.ui.AppBarConfiguration
import androidx.navigation.ui.NavigationUI
import com.superps2.emu.R
import com.superps2.emu.databinding.ActivityNavigationBinding
import java.util.*

class NavigationActivity : AppCompatActivity() {

    private lateinit var binding: ActivityNavigationBinding
    private lateinit var appBarConfiguration: AppBarConfiguration

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        // Inicializa o ViewBinding com o layout activity_navigation.xml
        binding = ActivityNavigationBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Configura a Toolbar como ActionBar com suporte ao tema
        setupToolbar()

        // Configura o Navigation Drawer e integra com NavController
        setupNavigation()

        // Carrega e exibe informações do dispositivo no cabeçalho do menu
        loadDeviceInfo()
    }

    /**
     * Configura a Toolbar para funcionar como ActionBar estilizada.
     */
    private fun setupToolbar() {
        setSupportActionBar(binding.appBarMain.toolbar)
    }

    /**
     * Conecta o NavigationView com o NavController e configura o botão de menu.
     */
    private fun setupNavigation() {
        val navController = findNavController(R.id.nav_host_fragment_container)

        // Define destinos de topo (sem botão de voltar)
        appBarConfiguration = AppBarConfiguration(
            setOf(
                R.id.nav_home,
                R.id.nav_settings,
                R.id.nav_change_language
            ), binding.drawerLayout
        )

        // Configura Toolbar + botão de hambúrguer com NavController
        NavigationUI.setupActionBarWithNavController(this, navController, appBarConfiguration)
        NavigationUI.setupWithNavController(binding.navView, navController)

        // Abertura/fechamento do Drawer via botão de menu
        val toggle = ActionBarDrawerToggle(
            this,
            binding.drawerLayout,
            binding.appBarMain.toolbar,
            R.string.navigation_drawer_open,
            R.string.navigation_drawer_close
        )
        binding.drawerLayout.addDrawerListener(toggle)
        toggle.syncState()
    }

    /**
     * Carrega informações básicas do dispositivo e calcula compatibilidade estimada.
     */
    private fun loadDeviceInfo() {
        val headerView = binding.navView.getHeaderView(0)

        val textDeviceName = headerView.findViewById<TextView>(R.id.text_device_name)
        val textDeviceInfo = headerView.findViewById<TextView>(R.id.text_device_info)

        val manufacturer = Build.MANUFACTURER.capitalize(Locale.ROOT)
        val model = Build.MODEL
        val deviceName = "$manufacturer $model"
        val chipset = Build.HARDWARE.uppercase(Locale.ROOT)

        val totalRamMB = getTotalMemoryMB()
        val compatibility = when {
            totalRamMB >= 4000 -> getString(R.string.compatibility_high)
            totalRamMB >= 2500 -> getString(R.string.compatibility_medium)
            else -> getString(R.string.compatibility_low)
        }

        // Preenche os textos do cabeçalho com informações coletadas
        textDeviceName.text = deviceName
        textDeviceInfo.text = getString(R.string.device_info_format, chipset, totalRamMB, compatibility)
    }

    /**
     * Recupera a memória RAM total do dispositivo em MB.
     */
    private fun getTotalMemoryMB(): Int {
        val actManager = getSystemService(Context.ACTIVITY_SERVICE) as ActivityManager
        val memInfo = ActivityManager.MemoryInfo()
        actManager.getMemoryInfo(memInfo)
        return (memInfo.totalMem / (1024 * 1024)).toInt()
    }

    /**
     * Suporte para o botão "voltar" integrado ao Navigation Drawer.
     */
    override fun onSupportNavigateUp(): Boolean {
        val navController = findNavController(R.id.nav_host_fragment_container)
        return NavigationUI.navigateUp(navController, appBarConfiguration) || super.onSupportNavigateUp()
    }

    /**
     * Fecha o Drawer ao pressionar o botão "voltar" se ele estiver aberto.
     */
    override fun onBackPressed() {
        if (binding.drawerLayout.isDrawerOpen(GravityCompat.START)) {
            binding.drawerLayout.closeDrawer(GravityCompat.START)
        } else {
            super.onBackPressed()
        }
    }
}
