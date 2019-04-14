#include <iostream>
#include <array>
#include <thread>
#include <random>

#define TABLE_SIZE 5

struct Fork {
    // ready for the next stage of the app
};

struct Table {
    std::atomic<bool> ready{false};
    std::array<Fork, TABLE_SIZE * 2> forks; // 2x because every person gets two separate forks
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
        std::cout << name << " stopped dining." << std::endl;
    }

    void dine() {
        while (!dinner_table.ready);

        do {
            think();
            eat();
        } while (dinner_table.ready);
    }

    void eat() {
        std::cout << name << " picked up right fork" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(25));

        std::cout << name << " picked up left fork" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(25));

        std::cout << name << " started eating" << std::endl;
        thread_local std::array<const char *, 3> foods {{"chicken", "rice", "soda"}};
        thread_local std::uniform_int_distribution<> wait(2, 4);
        thread_local std::uniform_int_distribution<> dist(0, foods.size() - 1);

        while (dist(random_generator) > 0) {
            std::cout << name << " is eating" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(wait(random_generator) * 50));
        }

        std::cout << name << " finished eating" << std::endl;
    }

    void think() {
        static constexpr std::array<const char *, 5> topics{{
                "politics", "art", "meaning of life", "source of morality", "how many straws makes a bale"
        }};

        thread_local std::uniform_int_distribution<> wait(2, 4);
        thread_local std::uniform_int_distribution<> dist(0, topics.size() - 1);

        std::cout << name << " started thinking" << std::endl;
        while (dist(random_generator) > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(wait(random_generator) * 150));
            std::cout << name << " is thinking about " << topics[dist(random_generator)] << std::endl;
        }

        std::cout << name << " finished thinking" << std::endl;
    }
};


void dinnertime(int time_limit) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "=== DINNER HAS STARTED ===" << std::endl;

    {
        Table table;
        std::array<Philosopher, TABLE_SIZE> philosophers {{  // threads are created using the constructor
                 {"Aristotle", table, table.forks[0], table.forks[1]},
                 {"Plato", table, table.forks[2], table.forks[3]},
                 {"Confucius", table, table.forks[4], table.forks[5]},
                 {"Socrates", table, table.forks[6], table.forks[7]},
                 {"Sun Tzu", table, table.forks[8], table.forks[9]},
         }};

        table.ready = true;
        std::this_thread::sleep_for(std::chrono::seconds(time_limit));
        table.ready = false;
        std::cout << "=== DINNER IS ENDING ===" << std::endl;
    }   // end of philosophers life (scope variable)

    std::cout << "=== DINNER IS OVER ===" << std::endl;
}

int main(int argc, char **argv) {
    int time_limit = 10;
    dinnertime(time_limit);

    return 0;
}
