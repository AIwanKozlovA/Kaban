#pragma once
#include <vector>
#include <chrono>
#include <algorithm>
#include <random>
#include <openssl/sha.h>
#include <iostream>

// Platform-specific headers
#ifdef _WIN32
#include <windows.h>
#include <wlanapi.h>
#include <iphlpapi.h>
#include <psapi.h>
#include <bcrypt.h>
#pragma comment(lib, "wlanapi.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "psapi.lib")
#elif __APPLE__
#include <CoreWLAN/CoreWLAN.h>
#include <sys/sysctl.h>
#include <net/route.h>
#include <net/if.h>
#include <mach/mach.h>
#include <ApplicationServices/ApplicationServices.h>
#else
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/wireless.h>
#include <fstream>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <dirent.h>
#include <cstring>
#include <sys/statvfs.h>
#include <fcntl.h>
#endif


#ifdef _WIN32
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")
#endif

using namespace std;
class EntropyCollector {
public:
    std::vector<uint8_t> collect() {
        std::vector<uint8_t> entropy;

        // Network statistics
        auto net_stats = get_network_stats();
        entropy.insert(entropy.end(), net_stats.begin(), net_stats.end());

        // Wi-Fi networks
        auto wifi_count = get_wifi_count();
        entropy.insert(entropy.end(), reinterpret_cast<uint8_t*>(&wifi_count), reinterpret_cast<uint8_t*>(&wifi_count + 1));

        // Cursor position
        auto cursor_pos = get_cursor_position();
        entropy.insert(entropy.end(), reinterpret_cast<uint8_t*>(&cursor_pos.first), reinterpret_cast<uint8_t*>(&cursor_pos.first + 1));
        entropy.insert(entropy.end(), reinterpret_cast<uint8_t*>(&cursor_pos.second), reinterpret_cast<uint8_t*>(&cursor_pos.second + 1));

        // Process count
        auto process_count = get_process_count();
        entropy.insert(entropy.end(), reinterpret_cast<uint8_t*>(&process_count), reinterpret_cast<uint8_t*>(&process_count + 1));

        // Memory usage
        auto memory_usage = get_memory_usage();
        entropy.insert(entropy.end(), reinterpret_cast<uint8_t*>(&memory_usage.first), reinterpret_cast<uint8_t*>(&memory_usage.first + 1));
        entropy.insert(entropy.end(), reinterpret_cast<uint8_t*>(&memory_usage.second), reinterpret_cast<uint8_t*>(&memory_usage.second + 1));

        // Новые источники
        add_cpu_temperature(entropy);
        add_cpu_frequency(entropy);
        add_disk_usage(entropy);
        add_battery_info(entropy);
        add_file_system_entropy(entropy);
        add_hardware_entropy(entropy);
        add_uptime(entropy);

        // Системная энтропия
        add_system_entropy(entropy);

        return entropy;
    }

private:
    // Добавление данных в вектор энтропии
    template<typename T>
    void add_data(std::vector<uint8_t>& data, const T& value) {
        const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&value);
        data.insert(data.end(), bytes, bytes + sizeof(T));
    }

    // 1. Температура CPU (только для Linux)
    void add_cpu_temperature(std::vector<uint8_t>& data) {
        int temp = 0;
#ifdef __linux__
        std::ifstream temp_file("/sys/class/thermal/thermal_zone0/temp");
        if (temp_file) {
            temp_file >> temp;
            temp /= 1000; // Конвертация в градусы
        }
#endif
        add_data(data, temp);
    }

    // 2. Частота процессора (в тактах)
    void add_cpu_frequency(std::vector<uint8_t>& data) {
        uint64_t cycles = 0;
#ifdef _WIN32
        cycles = __rdtsc();
#elif __APPLE__
        cycles = mach_absolute_time();
#else
        asm volatile ("rdtsc" : "=A" (cycles));
#endif
        add_data(data, cycles);
    }

    // 3. Использование диска
    void add_disk_usage(std::vector<uint8_t>& data) {
        uint64_t free_space = 0;
#ifdef _WIN32
        ULARGE_INTEGER free;
        GetDiskFreeSpaceExA("C:\\", &free, nullptr, nullptr);
        free_space = free.QuadPart;
#else
        struct statvfs stat;
        if (statvfs("/", &stat) == 0) {
            free_space = stat.f_bavail * stat.f_frsize;
        }
#endif
        add_data(data, free_space);
    }

    // 4. Информация о батарее
    void add_battery_info(std::vector<uint8_t>& data) {
        int battery_level = -1;
#ifdef _WIN32
        SYSTEM_POWER_STATUS status;
        if (GetSystemPowerStatus(&status)) {
            battery_level = status.BatteryLifePercent;
        }
#elif __APPLE__
        CFTypeRef blob = IOPSCopyPowerSourcesInfo();
        CFArrayRef sources = IOPSCopyPowerSourcesList(blob);
        if (CFArrayGetCount(sources) {
            CFDictionaryRef source = IOPSGetPowerSourceDescription(blob, CFArrayGetValueAtIndex(sources, 0));
            CFNumberRef level = (CFNumberRef)CFDictionaryGetValue(source, CFSTR(kIOPSCurrentCapacityKey));
            CFNumberGetValue(level, kCFNumberIntType, &battery_level);
        }
        CFRelease(sources);
            CFRelease(blob);
#else
        std::ifstream battery_file("/sys/class/power_supply/BAT0/capacity");
            if (battery_file) {
                battery_file >> battery_level;
            }
#endif
        add_data(data, battery_level);
    }

    // 5. Количество файлов во временной директории
    void add_file_system_entropy(std::vector<uint8_t>& data) {
        uint32_t file_count = 0;
#ifdef _WIN32
        WIN32_FIND_DATAA find_data;
        HANDLE hFind = FindFirstFileA("C:\\Windows\\Temp\\*", &find_data);
        if (hFind != INVALID_HANDLE_VALUE) {
            do {
                if (!(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                    file_count++;
                }
            } while (FindNextFileA(hFind, &find_data));
            FindClose(hFind);
        }
#else
        const char* tmp_dir = "/tmp";
        DIR* dir = opendir(tmp_dir);
        if (dir) {
            struct dirent* entry;
            while ((entry = readdir(dir)) != nullptr) {
                if (entry->d_type == DT_REG) file_count++;
            }
            closedir(dir);
        }
#endif
        add_data(data, file_count);
    }

    // 6. Аппаратные случайные данные
    void add_hardware_entropy(std::vector<uint8_t>& data) {
        uint8_t hw_entropy[32] = { 0 };
#ifdef _WIN32
       // BCryptGenRandom(nullptr, hw_entropy, sizeof(hw_entropy), BCRYPT_USE_SYSTEM_PREFERRED_RNG);
#elif __APPLE__
        int fd = open("/dev/random", O_RDONLY);
        if (fd >= 0) {
            read(fd, hw_entropy, sizeof(hw_entropy));
            close(fd);
        }
#else
        int fd = open("/dev/urandom", O_RDONLY);
        if (fd >= 0) {
            read(fd, hw_entropy, sizeof(hw_entropy));
            close(fd);
        }
#endif
        data.insert(data.end(), hw_entropy, hw_entropy + sizeof(hw_entropy));
    }

    // 7. Время работы системы
    void add_uptime(std::vector<uint8_t>& data) {
        uint64_t uptime = 0;
#ifdef _WIN32
        uptime = GetTickCount64();
#elif __APPLE__
        struct timeval boottime;
        size_t len = sizeof(boottime);
        sysctlbyname("kern.boottime", &boottime, &len, nullptr, 0);
        uptime = time(nullptr) - boottime.tv_sec;
#else
        std::ifstream uptime_file("/proc/uptime");
        if (uptime_file) {
            double seconds;
            uptime_file >> seconds;
            uptime = static_cast<uint64_t>(seconds);
        }
#endif
        add_data(data, uptime);
    }
    void add_system_entropy(std::vector<uint8_t>& data) {
        auto now = std::chrono::high_resolution_clock::now();
        auto time = now.time_since_epoch().count();
        data.insert(data.end(), reinterpret_cast<uint8_t*>(&time), reinterpret_cast<uint8_t*>(&time + 1));

#ifdef _WIN32
        DWORD pid = GetCurrentProcessId();
#else
        pid_t pid = getpid();
#endif
        data.insert(data.end(), reinterpret_cast<uint8_t*>(&pid), reinterpret_cast<uint8_t*>(&pid + 1));
    }

    std::vector<uint8_t> get_network_stats() {
        uint64_t tx = 0, rx = 0;

#ifdef _WIN32
        // Используем старый API GetIfTable для совместимости
        MIB_IFTABLE* pIfTable = nullptr;
        DWORD dwSize = 0;

        // Первый вызов - получаем необходимый размер буфера
        if (GetIfTable(pIfTable, &dwSize, FALSE) == ERROR_INSUFFICIENT_BUFFER) {
            pIfTable = (MIB_IFTABLE*)malloc(dwSize);
            if (pIfTable) {
                // Второй вызов - получаем данные
                if (GetIfTable(pIfTable, &dwSize, FALSE) == NO_ERROR) {
                    for (DWORD i = 0; i < pIfTable->dwNumEntries; i++) {
                        tx += pIfTable->table[i].dwOutOctets;
                        rx += pIfTable->table[i].dwInOctets;
                    }
                }
                free(pIfTable);
            }
        }
#elif __APPLE__
        int mib[] = { CTL_NET, PF_ROUTE, 0, 0, NET_RT_IFLIST2, 0 };
        size_t len;
        if (sysctl(mib, 6, NULL, &len, NULL, 0) == 0) {
            std::vector<uint8_t> buf(len);
            if (sysctl(mib, 6, buf.data(), &len, NULL, 0) == 0) {
                auto ifm = (struct if_msghdr2*)buf.data();
                for (size_t i = 0; i < len; i += ifm->ifm_msglen) {
                    ifm = (struct if_msghdr2*)(buf.data() + i);
                    if (ifm->ifm_type == RTM_IFINFO2) {
                        tx += ifm->ifm_data.ifi_obytes;
                        rx += ifm->ifm_data.ifi_ibytes;
                    }
                }
            }
        }
#else
        std::ifstream net("/proc/net/dev");
        std::string line;
        while (std::getline(net, line)) {
            if (line.find(":") != std::string::npos) {
                uint64_t r, t;
                sscanf(line.c_str(), "%*s %lu %*u %*u %*u %*u %*u %*u %lu", &r, &t);
                rx += r;
                tx += t;
            }
        }
#endif

        std::vector<uint8_t> result;
        result.insert(result.end(), reinterpret_cast<uint8_t*>(&tx), reinterpret_cast<uint8_t*>(&tx + 1));
        result.insert(result.end(), reinterpret_cast<uint8_t*>(&rx), reinterpret_cast<uint8_t*>(&rx + 1));
        return result;
    }

    uint32_t get_wifi_count() {
        uint32_t count = 0;

#ifdef _WIN32
        /*HANDLE hWlan;
        DWORD version;
        if (WlanOpenHandle(2, NULL, &version, &hWlan) == ERROR_SUCCESS) {
            PWLAN_BSS_LIST pBssList = NULL;
            // Указываем NULL для всех параметров, чтобы получить список всех сетей
            if (WlanGetNetworkBssList(hWlan,
                NULL,    // Интерфейс (все интерфейсы)
                NULL,    // DOT11_SSID (все SSID)
                dot11_BSS_type_any,
                false,   // Безопасные данные
                NULL,    // Дополнительные параметры
                &pBssList) == ERROR_SUCCESS) {
                if (pBssList) {
                    count = pBssList->dwNumberOfItems;
                    WlanFreeMemory(pBssList);
                }
                }
                WlanCloseHandle(hWlan, NULL);*/
                // Альтернатива: использовать количество сетевых интерфейсов
        DWORD adapterCount = 0;
        GetNumberOfInterfaces(&adapterCount);
        count = adapterCount;
#elif __APPLE__
        CWInterface* wifi = [CWInterface interfaceWithName : nil];
        NSSet* networks = [wifi scanForNetworksWithName : nil error : nil];
        count = (uint32_t)[networks count];
#else
        int sock = socket(AF_INET, SOCK_DGRAM, 0);
        struct iwreq wrq;
        char buffer[4096];
        wrq.u.data.pointer = buffer;
        wrq.u.data.length = sizeof(buffer);
        wrq.u.data.flags = 0;
        if (ioctl(sock, SIOCGIWSCAN, &wrq) != -1) {
            struct iw_event iwe;
            char* pos = buffer;
            while (pos + sizeof(iwe) <= buffer + wrq.u.data.length) {
                memcpy(&iwe, pos, sizeof(iwe));
                if (iwe.cmd == SIOCGIWAP) {
                    count++;
                }
                pos += iwe.len;
            }
        }
        close(sock);
#endif

        return count;
    }

    std::pair<int, int> get_cursor_position() {
        int x = 0, y = 0;

#ifdef _WIN32
        POINT cursorPos;
        if (GetCursorPos(&cursorPos)) {
            x = cursorPos.x;
            y = cursorPos.y;
        }
#elif __APPLE__
        CGEventRef event = CGEventCreate(NULL);
        CGPoint cursor = CGEventGetLocation(event);
        CFRelease(event);
        x = static_cast<int>(cursor.x);
        y = static_cast<int>(cursor.y);
#else
        // Linux: No direct API, can use X11 if available
        // For simplicity, we'll return (0, 0) here
#endif

        return { x, y };
    }

    uint32_t get_process_count() {
        uint32_t count = 0;

#ifdef _WIN32
        DWORD processes[1024], cbNeeded;
        if (EnumProcesses(processes, sizeof(processes), &cbNeeded)) {
            count = cbNeeded / sizeof(DWORD);
        }
#elif __APPLE__
        int mib[] = { CTL_KERN, KERN_PROC, KERN_PROC_ALL, 0 };
        size_t len;
        if (sysctl(mib, 4, NULL, &len, NULL, 0) == 0) {
            std::vector<uint8_t> buf(len);
            if (sysctl(mib, 4, buf.data(), &len, NULL, 0) == 0) {
                count = len / sizeof(struct kinfo_proc);
            }
        }
#else
        DIR* dir = opendir("/proc");
        if (dir) {
            while (readdir(dir)) {
                count++;
            }
            closedir(dir);
        }
#endif

        return count;
    }

    std::pair<uint64_t, uint64_t> get_memory_usage() {
        uint64_t used = 0, free = 0;

#ifdef _WIN32
        MEMORYSTATUSEX memInfo;
        memInfo.dwLength = sizeof(MEMORYSTATUSEX);
        if (GlobalMemoryStatusEx(&memInfo)) {
            used = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
            free = memInfo.ullAvailPhys;
        }
#elif __APPLE__
        vm_size_t pageSize;
        mach_port_t machPort = mach_host_self();
        vm_statistics64_data_t vmStats;
        mach_msg_type_number_t count = sizeof(vmStats) / sizeof(natural_t);
        if (host_page_size(machPort, &pageSize) == KERN_SUCCESS &&
            host_statistics64(machPort, HOST_VM_INFO, reinterpret_cast<host_info64_t>(&vmStats), &count) == KERN_SUCCESS) {
            used = (vmStats.active_count + vmStats.wire_count) * pageSize;
            free = vmStats.free_count * pageSize;
        }
#else
        struct sysinfo memInfo;
        if (sysinfo(&memInfo) == 0) {
            used = (memInfo.totalram - memInfo.freeram) * memInfo.mem_unit;
            free = memInfo.freeram * memInfo.mem_unit;
        }
#endif

        return { used, free };
    }
};
