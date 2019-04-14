#include <iostream>
#include <array>
#include <thread>
#include <random>

constexpr int table_size = 5;
std::mutex cout_mutex;

struct Synchronization {
    std::mutex mutex;
    std::condition_variable condition_variable;

public:
    void wait() {
        std::unique_lock<std::mutex> lock(mutex);
        condition_variable.wait(lock);
    }

    void notify_all() {
        std::unique_lock<std::mutex> lock(mutex);
        condition_variable.notify_all();
    }
};

struct Fork {
    const int id;
    int owner;
    bool dirty;
    std::mutex mutex;
    Synchronization synchronization;

public:
    Fork(const int fork_id, const int owner_id) :
            id(fork_id), owner(owner_id), dirty(true) {}

    void request(int const owner_id) {
        while (owner != owner_id) {
            if (dirty) {
                std::lock_guard<std::mutex> lock(mutex);
                dirty = false;
                owner = owner_id;
            } else {
                synchronization.wait();
            }
        }
    }

    void done_using() {
        dirty = true;
        synchronization.notify_all();
    }
};

struct Table {
    std::atomic<bool> ready{false};
    Synchronization synchronization;

    ~Table() {
        std::cout << "=== DINNER IS OVER ===" << std::endl;
    }
};

struct Philosopher {
private:
    std::mt19937 random_generator{std::random_device{}()};

    const int id;
    const std::string name;
    Table &dinner_table;
    Fork &left_fork;
    Fork &right_fork;
    std::thread dinner_thread;

public:
    Philosopher(const int id, std::string const &name, Table &table, Fork &left, Fork &right) :
            id(id), name(name), dinner_table(table), left_fork(left), right_fork(right),
            dinner_thread(&Philosopher::dine, this) {}

    ~Philosopher() {
        dinner_thread.join();
        std::lock_guard<std::mutex> cout_lock(cout_mutex);
        std::cout << name << " stopped dining." << std::endl;
    }

    void dine() {
        dinner_table.synchronization.wait();

        do {
            eat();
            think();
        } while (dinner_table.ready);
    }

    void eat() {
        left_fork.request(id);
        right_fork.request(id);

        std::lock(left_fork.mutex, right_fork.mutex);   // ensures there are no deadlocks

        // std::adopt_lock parameter indicates that the mutexes are already locked and they should adopt the ownership of the existing lock
        std::lock_guard<std::mutex>  left_lock(left_fork.mutex, std::adopt_lock);
        std::lock_guard<std::mutex> right_lock(right_fork.mutex, std::adopt_lock);
        std::this_thread::sleep_for(std::chrono::milliseconds(25));

        // picked up forks
        {
            std::lock_guard<std::mutex> cout_lock(cout_mutex);
            std::cout << name << " started eating" << std::endl;
        }

        thread_local std::array<const char *, 3> foods {{"chicken", "rice", "soda"}};
        thread_local std::uniform_int_distribution<> wait(2, 4);
        thread_local std::uniform_int_distribution<> dist(0, foods.size() - 1);

        while (dist(random_generator) > 0) {
            std::lock_guard<std::mutex>  cout_lock(cout_mutex);
            std::cout << name << " is eating " << foods[dist(random_generator)] << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(wait(random_generator) * 50));
        }

        left_fork.done_using();
        right_fork.done_using();
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
        std::array<Fork, table_size> forks {{
            {1, 1},
            {2, 2},
            {3, 3},
            {4, 4},
            {5, 1}
        }};

        std::array<Philosopher, table_size> philosophers {{  // threads are created using the constructor
            {1, "Aristotle", table, forks[0], forks[1]},
            {2, "Plato", table, forks[1], forks[2]},
            {3, "Confucius", table, forks[2], forks[3]},
            {4, "Socrates", table, forks[3], forks[4]},
            {5, "Sun Tzu", table, forks[4], forks[0]},
        }};
        std::this_thread::sleep_for(std::chrono::seconds(1));
        table.ready = true;
        table.synchronization.notify_all();
        std::this_thread::sleep_for(std::chrono::seconds(time_limit));
        table.ready = false;
        std::lock_guard<std::mutex> cout_lock(cout_mutex);
        std::cout << "=== DINNER IS ENDING ===" << std::endl;
    }   // end of philosophers life (scope variable)
}

int main(int argc, char **argv) {
    int time_limit = 5;
    dinnertime(time_limit);

    return 0;
}
