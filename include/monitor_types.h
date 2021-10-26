#ifndef MONITOR_TYPES_H
#define MONITOR_TYPES_H

namespace SysMon
{


struct CpuTime {
    int user;
    int nice;
    int system;
    int idle;
    int iowait;
    int irq;
    int softirq;

    CpuTime operator-(CpuTime cpuTime) {
        cpuTime.user = user - cpuTime.user;
        cpuTime.nice = nice - cpuTime.nice;
        cpuTime.system = system - cpuTime.system;
        cpuTime.idle = idle - cpuTime.idle;
        cpuTime.iowait = iowait - cpuTime.iowait;
        cpuTime.softirq = softirq - cpuTime.softirq;

        return cpuTime;
    }

    int Total() {
        return user + nice + system + idle + iowait + irq + softirq;
    }
};

struct CpuUtilization {
    long utime;
    long stime;
    long cutime;
    long cstime;
    long starttime;
    long utilization;
    bool running;

    float total;
};

struct Ram {
    int rss;
};

struct Passwd {
    std::string user;
    int UID;
    int GID;
};
}

#endif