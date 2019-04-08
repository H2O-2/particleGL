const float DEFAULT_GRAVITY = 0.0f;

struct Physics {
    float gravity;

    static Physics& getInstance() {
        static Physics instance;

        return instance;
    }
    Physics(const Physics&) = delete;
    Physics& operator=(const Physics&) = delete;
private:
    Physics() : gravity(DEFAULT_GRAVITY) {}
};
