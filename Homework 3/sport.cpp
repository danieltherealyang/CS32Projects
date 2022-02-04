class Sport {
    public:
        Sport(string name) : m_name(name) {};
        virtual ~Sport() = 0;
        string name() const;
        virtual bool isOutdoor() const;
        virtual string icon() const = 0;
    private:
        string m_name;
};

Sport::~Sport() {
}

string Sport::name() const {
    return m_name;
}

bool Sport::isOutdoor() const {
    return true;
}

class Snowboarding : public Sport {
    public:
        Snowboarding(string name) : Sport(name) {};
        virtual ~Snowboarding();
        virtual string icon() const;
};

Snowboarding::~Snowboarding() {
    cout << "Destroying the Snowboarding object named " << name() << "." << endl;
}

string Snowboarding::icon() const {
    return "a descending snowboarder";
}

class Biathlon : public Sport {
    public:
        Biathlon(string name, float distance);
        virtual ~Biathlon();
        virtual string icon() const;
    private:
        float m_distance;
};

Biathlon::Biathlon(string name, float distance) : Sport(name), m_distance(distance) {
}

Biathlon::~Biathlon() {
    cout << "Destroying the Biathlon object named " << name() << ", distance " << m_distance << " km." << endl;
}

string Biathlon::icon() const {
    return "a skier with a rifle";
}

class FigureSkating : public Sport {
    public:
        FigureSkating(string name) : Sport(name) {};
        virtual ~FigureSkating();
        virtual bool isOutdoor() const;
        virtual string icon() const;
};

FigureSkating::~FigureSkating() {
    cout << "Destroying the FigureSkating object named " << name() << "." << endl;
}

bool FigureSkating::isOutdoor() const {return false;}

string FigureSkating::icon() const {
    return "a skater in the Biellmann position";
}