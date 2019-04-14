#include <iostream>
#include <array>
#include <thread>
#include <random>

#define TABLE_SIZE 5

std::mutex cout_mutex;

struct Fork {
    std::mutex mutex;
};

struct Table {
    std::atomic<bool> ready{false};
    std::array<Fork, TABLE_SIZE> forks; // every person gets one fork

    ~Table() {
        std::cout << "=== DINNER IS OVER ===" << std::endl;
    }
};

struct Philosopher {
private:
    std::mt19937 random_generator{std::random_device{}()};

    const std::string name;
    const Table &dinner_table;
    Fork &left_fork;
    Fork &right_fork;
    std::thread dinner_thread;

public:
    Philosopher(const std::string &name, Table const &table, Fork &left, Fork &right) :
            name(name), dinner_table(table), left_fork(left), right_fork(right),
            dinner_thread(&Philosopher::dine, this) {
    }

    ~Philosopher() {
        dinner_thread.join();
        std::lock_guard<std::mutex> cout_lock(cout_mutex);
        std::cout << name << " stopped dining." << std::endl;
    }

    void dine() {
        while (not dinner_table.ready);

        do {
            std::lock(left_fork.mutex, right_fork.mutex);   // ensures there are no deadlocks
            eat();
            if (not dinner_table.ready) break;
            think();
        } while (dinner_table.ready);
    }

    void eat() {
        // std::adopt_lock parameter indicates that the mutexes are already locked and they should adopt the ownership of the existing lock
        std::lock_guard<std::mutex>  left_lock(left_fork.mutex, std::adopt_lock);
        std::lock_guard<std::mutex> right_lock(right_fork.mutex, std::adopt_lock);
        std::this_thread::sleep_for(std::chrono::milliseconds(25));

        // picked up forks

        if (not dinner_table.ready) return;
        {
            std::lock_guard<std::mutex> cout_lock(cout_mutex);
            std::cout << name << " started eating" << std::endl;
        }

        thread_local std::array<const char *, 3> foods {{"chicken", "rice", "soda"}};
        thread_local std::uniform_int_distribution<> wait(2, 4);
        thread_local std::uniform_int_distribution<> dist(0, foods.size() - 1);

        while (dist(random_generator) > 0) {
            std::lock_guard<std::mutex>  cout_lock(cout_mutex);
            std::cout << name << " is eating" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(wait(random_generator) * 50));
        }

        std::lock_guard<std::mutex> cout_lock(cout_mutex);
        std::cout << name << " finished eating" << std::endl;
    }

    void think() {
        static constexpr std::array<const char *, 5> topics{{
                "politics", "art", "the meaning of life", "the source of morality", "how many straws makes a bale"
        }};

        thread_local std::uniform_int_distribution<> wait(2, 4);
        thread_local std::uniform_int_distribution<> dist(0, topics.size() - 1);

        std::cout << name << " started thinking" << std::endl;
        while (dist(random_generator) > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(wait(random_generator) * 150));
            std::lock_guard<std::mutex> cout_lock(cout_mutex);
            std::cout << name << " is thinking about " << topics[dist(random_generator)] << std::endl;
            if(not dinner_table.ready) return;
        }

        std::lock_guard<std::mutex> cout_lock(cout_mutex);
        std::cout << name << " finished thinking" << std::endl;
    }
};


void dinnertime(int time_limit) {
    std::cout << "=== DINNER HAS STARTED ===" << std::endl;
    {
        Table table;
        std::array<Philosopher, TABLE_SIZE> philosophers {{  // threads are created using the constructor
            {"Aristotle", table, table.forks[0], table.forks[1]},
            {"Plato", table, table.forks[1], table.forks[2]},
            {"Confucius", table, table.forks[2], table.forks[3]},
            {"Socrates", table, table.forks[3], table.forks[4]},
            {"Sun Tzu", table, table.forks[4], table.forks[0]},
        }};
        std::this_thread::sleep_for(std::chrono::seconds(1));
        table.ready = true;
        std::this_thread::sleep_for(std::chrono::seconds(time_limit));
        table.ready = false;
        std::lock_guard<std::mutex> cout_lock(cout_mutex);
        std::cout << "=== DINNER IS ENDING ===" << std::endl;
    }   // end of philosophers life (scope variable)
}

int main(int argc, char **argv) {
    int time_limit = 10;
    dinnertime(time_limit);

    return 0;
}
