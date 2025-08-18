/******************************************************************************************
 * © Copyright [2025] VESTEL Corporation. All Rights Reserved.
 *
 * File Name    : her2_monitor_fw_sim.c
 * Author       : Gokhan Akkaya
 * Device(s)    : PC Simulation
 * Tool-Chain   : MSVC / MinGW
 * Description  : Simulation of the Thread Monitor Framework Module.
 * Project      : VESTEL Renesas Synergy Family HAL Layer.
 * Creation Date: 28.03.25
 ******************************************************************************************/

 /*------------------------------------ Includes ------------------------------------------*/

#include "her2_monitor_fw.h"

#if(MONITOR_FW)
#include "tx_api.h"

/*------------------------------ Private Typedef Definitions  ----------------------------*/

// Structure to hold monitoring information for each registered thread.
typedef struct
{
    TX_THREAD* p_thread;
    ULONG minimum_count_ticks;
    ULONG maximum_count_ticks;
    ULONG last_increment_time_ticks;
    bool is_registered;
    char* thread_name;
} monitored_thread_info_t;

/*----------------------------- Private Constant & Macro ----------------------------------*/

#ifndef THREAD_MONITOR_CFG_MAX_NUMBER_OF_THREADS
#define THREAD_MONITOR_CFG_MAX_NUMBER_OF_THREADS 8 // Max threads to monitor
#endif

#define WATCHDOG_THREAD_STACK_SIZE 1024
#define WATCHDOG_CHECK_INTERVAL_TICKS 100 // Check threads every 100 ticks (e.g., 100ms)

/*------------------------------------ Static Variables ----------------------------------*/

static TX_MUTEX monfw_rdy_lock;
static bool g_is_configured = false;

// Array to store the "report cards" of monitored threads.
static monitored_thread_info_t g_monitored_threads[THREAD_MONITOR_CFG_MAX_NUMBER_OF_THREADS];

// Watchdog thread that checks the other threads.
static TX_THREAD g_watchdog_thread;
static UCHAR g_watchdog_stack[WATCHDOG_THREAD_STACK_SIZE];

#if defined(WDT_CALLBACK)
static void (*wdt_event)(void) = NULL;
#endif

/*------------------------------- Private Function Prototypes ----------------------------*/

static void watchdog_thread_entry(ULONG thread_input);

/*----------------------------------- Private Functions ----------------------------------*/

/******************************************************************************************
 * Function Name: watchdog_thread_entry
 * Description  : This thread periodically wakes up and checks if any monitored
 *                threads have timed out.
 *****************************************************************************************/
static void watchdog_thread_entry(ULONG thread_input)
{
    (void)thread_input;
    while (1)
    {
        tx_thread_sleep(WATCHDOG_CHECK_INTERVAL_TICKS);

        tx_mutex_get(&monfw_rdy_lock, TX_WAIT_FOREVER);

        ULONG current_ticks = tx_time_get();

        for (uint32_t i = 0; i < THREAD_MONITOR_CFG_MAX_NUMBER_OF_THREADS; i++)
        {
            if (g_monitored_threads[i].is_registered)
            {
                ULONG elapsed_ticks = current_ticks - g_monitored_threads[i].last_increment_time_ticks;

                if (elapsed_ticks > g_monitored_threads[i].maximum_count_ticks)
                {
                    // This thread has timed out!
                    printf("[SIM-MONITOR] ERROR: Thread '%s' has timed out! (Elapsed: %lu > Max: %lu)\n",
                        g_monitored_threads[i].thread_name, elapsed_ticks, g_monitored_threads[i].maximum_count_ticks);

                    // Trigger the simulated WDT event.
#if defined(WDT_CALLBACK)
                    if (wdt_event)
                    {
                        wdt_event();
                    }
#endif

                    // In a real scenario, you might want to stop the simulation or log this.
                    // For this simulation, we'll unregister it to prevent repeated messages.
                    g_monitored_threads[i].is_registered = false;
                }
            }
        }
        tx_mutex_put(&monfw_rdy_lock);
    }
}

/*----------------------------------- Public Functions -----------------------------------*/

#if defined(WDT_CALLBACK)
void WDT_CALLBACK(void* p_args) { /* This would be the hardware callback, not needed in sim */ }
void wdt_callback_configure(void (*p_wdt_callback)(void)) { wdt_event = p_wdt_callback; }
#endif

app_err_t monitor_fw_configure(void)
{
    if (g_is_configured) 
        return APP_SUCCESS;

    memset(g_monitored_threads, 0, sizeof(g_monitored_threads));

    UINT status = tx_mutex_create(&monfw_rdy_lock, "Monitor FW Lock", TX_INHERIT);
    if (status != TX_SUCCESS) 
        return APP_ERR_THREADX;

    // Create and start the watchdog thread
    status = tx_thread_create(&g_watchdog_thread, "Watchdog Thread",
        watchdog_thread_entry, 0,
        g_watchdog_stack, WATCHDOG_THREAD_STACK_SIZE,
        1, 1, TX_NO_TIME_SLICE, TX_AUTO_START); // High priority
    if (status != TX_SUCCESS)
    {
        tx_mutex_delete(&monfw_rdy_lock);
        return APP_ERR_THREADX;
    }

    g_is_configured = true;
    return APP_SUCCESS;
}

app_err_t monitor_fw_get_counts(thread_monitor_min_max* p_min_max_values)
{
    // This function's original purpose is complex to simulate.
    // In simulation, we can interpret it as "get the elapsed time since last check-in".
    if (p_min_max_values == NULL) return APP_ERR_ASSERTION;

    tx_mutex_get(&monfw_rdy_lock, TX_WAIT_FOREVER);

    TX_THREAD* current_thread = tx_thread_identify();
    ULONG current_ticks = tx_time_get();

    for (uint32_t i = 0; i < THREAD_MONITOR_CFG_MAX_NUMBER_OF_THREADS; i++)
    {
        if (g_monitored_threads[i].is_registered && g_monitored_threads[i].p_thread == current_thread)
        {
            p_min_max_values->minimum_count = 0; // Not easily simulated, return 0
            p_min_max_values->maximum_count = current_ticks - g_monitored_threads[i].last_increment_time_ticks;
            break;
        }
    }

    tx_mutex_put(&monfw_rdy_lock);
    return APP_SUCCESS;
}

app_err_t monitor_fw_thread_register(thread_monitor_min_max* p_min_max_values)
{
    if (!g_is_configured) return APP_ERR_DRV_CONFIGURE;
    if (p_min_max_values == NULL) return APP_ERR_ASSERTION;

    UINT tx_res = tx_mutex_get(&monfw_rdy_lock, TX_WAIT_FOREVER);

    app_err_t result = APP_SUCCESS;
    TX_THREAD* current_thread = tx_thread_identify();
    int empty_slot = -1;

    // Find an empty slot to register the new thread
    for (int i = 0; i < THREAD_MONITOR_CFG_MAX_NUMBER_OF_THREADS; i++)
    {
        if (!g_monitored_threads[i].is_registered)
        {
            empty_slot = i;
            break;
        }
    }

    if (empty_slot != -1)
    {
        g_monitored_threads[empty_slot].p_thread = current_thread;
        g_monitored_threads[empty_slot].minimum_count_ticks = p_min_max_values->minimum_count;
        g_monitored_threads[empty_slot].maximum_count_ticks = p_min_max_values->maximum_count;
        g_monitored_threads[empty_slot].last_increment_time_ticks = tx_time_get();
        g_monitored_threads[empty_slot].thread_name = current_thread->tx_thread_name;
        g_monitored_threads[empty_slot].is_registered = true;
        printf("[SIM-MONITOR] Thread '%s' registered for monitoring.\n", current_thread->tx_thread_name);
    }
    else
    {
        result = APP_FAIL; // No more slots available
    }

    tx_mutex_put(&monfw_rdy_lock);
    return result;
}

app_err_t monitor_fw_thread_unregister(void)
{
    if (!g_is_configured) return APP_ERR_DRV_CONFIGURE;

    tx_mutex_get(&monfw_rdy_lock, TX_WAIT_FOREVER);

    TX_THREAD* current_thread = tx_thread_identify();

    for (int i = 0; i < THREAD_MONITOR_CFG_MAX_NUMBER_OF_THREADS; i++)
    {
        if (g_monitored_threads[i].is_registered && g_monitored_threads[i].p_thread == current_thread)
        {
            g_monitored_threads[i].is_registered = false;
            printf("[SIM-MONITOR] Thread '%s' unregistered.\n", current_thread->tx_thread_name);
            break;
        }
    }

    tx_mutex_put(&monfw_rdy_lock);
    return APP_SUCCESS;
}

app_err_t monitor_fw_count_increment(void)
{
    if (!g_is_configured) return APP_ERR_DRV_CONFIGURE;

    tx_mutex_get(&monfw_rdy_lock, TX_WAIT_FOREVER);

    TX_THREAD* current_thread = tx_thread_identify();
    bool found = false;

    for (int i = 0; i < THREAD_MONITOR_CFG_MAX_NUMBER_OF_THREADS; i++)
    {
        if (g_monitored_threads[i].is_registered && g_monitored_threads[i].p_thread == current_thread)
        {
            g_monitored_threads[i].last_increment_time_ticks = tx_time_get();
            found = true;
            break;
        }
    }

    tx_mutex_put(&monfw_rdy_lock);

    return found ? APP_SUCCESS : APP_FAIL; // Return fail if thread was not registered
}

#endif