#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>

// Class to log pizza orders in text file.
class Logger
{
public:
    static Logger &instance()
    {
        static Logger logger;
        return logger;
    }

    void write(std::string data)
    {
        time_t seconds = time(NULL);
        tm timeinfo = *localtime(&seconds);
        file << data << " " << asctime(&timeinfo) << "\n";
    }

private:
    Logger()
    {
        file.open("log.txt", std::ios::app);
    }

    Logger(const Logger &logger) = delete;

    Logger &operator=(const Logger &logger) = delete;

    ~Logger()
    {
        file.close();
    }

    std::ofstream file;
};

// Ingredient.
class AbstractIngredient
{
public:
    virtual std::string get() const = 0;
};

// Cheese.
class AbstractCheese : public AbstractIngredient
{
public:
    virtual std::string get() const = 0;
};

class BrieCheese : public AbstractCheese
{
    std::string get() const override
    {
        return "Brie Cheese";
    };
};

class MaccagnoCheese : public AbstractCheese
{
    std::string get() const override
    {
        return "Maccagno Cheese";
    };
};

class MozzarellaCheese : public AbstractCheese
{
    std::string get() const override
    {
        return "Mozzarella Cheese";
    };
};

// Veggies.
class AbstractVeggie : public AbstractIngredient
{
public:
    virtual std::string get() const = 0;
};

class Mushroom : public AbstractVeggie
{
public:
    std::string get() const override
    {
        return "Mushrooms";
    };
};

class Onion : public AbstractVeggie
{
public:
    std::string get() const override
    {
        return "Onion";
    };
};

class Spinach : public AbstractVeggie
{
public:
    std::string get() const override
    {
        return "Spinach";
    };
};

// Sauce.
class AbstractSauce : public AbstractIngredient
{
public:
    virtual std::string get() const = 0;
};

class MarinaraSauce : public AbstractSauce
{
public:
    std::string get() const override
    {
        return "Marinara Sauce";
    };
};

class TomatoSauce : public AbstractSauce
{
public:
    std::string get() const override
    {
        return "Tomato Sauce";
    };
};

// Ingredient factory.
class AbstractIngredientFactory
{
public:
    virtual AbstractCheese *createCheese() = 0;
    virtual AbstractSauce *createSauce() = 0;
    virtual std::vector<AbstractVeggie *> *createVeggies() = 0;
};

class NYIngredientFactory : public AbstractIngredientFactory
{
public:
    AbstractCheese *createCheese() override
    {
        return new MaccagnoCheese;
    }

    AbstractSauce *createSauce() override
    {
        return new MarinaraSauce;
    }

    std::vector<AbstractVeggie *> *createVeggies() override
    {
        return new std::vector<AbstractVeggie *>{
            new Onion,
            new Mushroom};
    }
};

class MoscowIngredientFactory : public AbstractIngredientFactory
{
public:
    AbstractCheese *createCheese() override
    {
        return new BrieCheese;
    }

    AbstractSauce *createSauce() override
    {
        return new TomatoSauce;
    }

    std::vector<AbstractVeggie *> *createVeggies() override
    {
        return new std::vector<AbstractVeggie *>{
            new Spinach,
            new Mushroom,
            new Onion};
    }
};

// Pizza.
class AbstractPizza
{
public:
    std::string bake() const
    {
        return "Bake for 25 minutes at 350";
    }

    std::string cut() const
    {
        return "Cutting the pizza into diagonal slices";
    }

    std::string box() const
    {
        return "Place pizza in official PizzaStore box";
    }

    void setName(const std::string &name)
    {
        _name = name;
    }

    std::string getName() const
    {
        return _name;
    }

    std::string get() const
    {
        std::string str{};

        if (!_name.empty())
        {
            str.append("\tName: " + _name + "\n");
        }

        if (_mainIngredient)
        {
            str.append("\t! Main ingredient: " + _mainIngredient->get() + "\n");
        }

        if (_cheese)
        {
            str.append("\tCheese: " + _cheese->get() + "\n");
        }

        if (_sauce)
        {
            str.append("\tSauce: " + _sauce->get() + "\n");
        }

        if (_veggies)
        {
            str.append("\tVeggies: ");
            for (auto &veggie : *_veggies)
            {
                str.append(veggie->get() + " ");
            }
        }

        return str;
    };

    void setMainIngredient(AbstractIngredient *mainIngredient)
    {
        _mainIngredient = mainIngredient;
    }

    void setVeggies(std::vector<AbstractVeggie *> *veggies)
    {
        _veggies = veggies;
    }

    void setCheese(AbstractCheese *cheese)
    {
        _cheese = cheese;
    }

    void setSauce(AbstractSauce *sauce)
    {
        _sauce = sauce;
    }

    virtual std::string prepare() = 0;

private:
    std::string _name;
    AbstractIngredient *_mainIngredient;
    std::vector<AbstractVeggie *> *_veggies;
    AbstractCheese *_cheese;
    AbstractSauce *_sauce;
};

class CheesePizza : public AbstractPizza
{
public:
    CheesePizza(AbstractIngredientFactory *ingredientFactory)
        : _ingredientFactory{ingredientFactory}
    {
        setMainIngredient(new MozzarellaCheese);
    }

    std::string prepare() override
    {
        setVeggies(_ingredientFactory->createVeggies());
        setCheese(_ingredientFactory->createCheese());
        setSauce(_ingredientFactory->createSauce());

        return "~~~ Preparing pizza... ~~~\n" + get();
    }

private:
    AbstractIngredientFactory *_ingredientFactory;
};

class BraccioDiFerroPizza : public AbstractPizza
{
public:
    BraccioDiFerroPizza(AbstractIngredientFactory *ingredientFactory)
        : _ingredientFactory{ingredientFactory}
    {
        setMainIngredient(new Mushroom);
    }

    std::string prepare() override
    {
        setVeggies(_ingredientFactory->createVeggies());
        setCheese(_ingredientFactory->createCheese());
        setSauce(_ingredientFactory->createSauce());

        return "~~~ Preparing pizza... ~~~\n" + get();
    }

private:
    AbstractIngredientFactory *_ingredientFactory;
};

// Pizza Types.
enum class PizzaTypes
{
    CHEESE,
    BRACCIODIFERRO
};

// Pizza stores.
class AbstractPizzaStore
{
public:
    AbstractPizza *orderPizza(PizzaTypes type)
    {
        AbstractPizza *pizza = createPizza(type);

        std::string str{};
        str += "~~~ Making a " + pizza->getName() + "\n" + pizza->bake() + "\n" + pizza->cut() + "\n" + pizza->box() + "\n" + pizza->get() + "\n";

        Logger::instance().write(str);

        return pizza;
    }

protected:
    virtual AbstractPizza *createPizza(PizzaTypes type) = 0;
};

class NYPizzaStore : public AbstractPizzaStore
{
protected:
    virtual AbstractPizza *createPizza(PizzaTypes type) override
    {
        AbstractPizza *pizza = nullptr;
        AbstractIngredientFactory *ingredientFactory = new NYIngredientFactory;

        if (type == PizzaTypes::CHEESE)
        {
            pizza = new CheesePizza(ingredientFactory);
            pizza->setName("NY Cheese Pizza");
            pizza->prepare();
        }
        if (type == PizzaTypes::BRACCIODIFERRO)
        {
            pizza = new BraccioDiFerroPizza(ingredientFactory);
            pizza->setName("NY Braccio di Ferro Pizza");
            pizza->prepare();
        }

        return pizza;
    }
};

class MoscowPizzaStore : public AbstractPizzaStore
{
protected:
    virtual AbstractPizza *createPizza(PizzaTypes type) override
    {
        AbstractPizza *pizza = nullptr;
        AbstractIngredientFactory *ingredientFactory = new MoscowIngredientFactory;

        if (type == PizzaTypes::CHEESE)
        {
            pizza = new CheesePizza(ingredientFactory);
            pizza->setName("Moscow Cheese Pizza");
            pizza->prepare();
        }
        if (type == PizzaTypes::BRACCIODIFERRO)
        {
            pizza = new BraccioDiFerroPizza(ingredientFactory);
            pizza->setName("Moscow Braccio di Ferro Pizza");
            pizza->prepare();
        }

        return pizza;
    }
};

int main()
{
    AbstractPizzaStore *NYStore = new NYPizzaStore;
    NYStore->orderPizza(PizzaTypes::CHEESE);
    NYStore->orderPizza(PizzaTypes::BRACCIODIFERRO);

    AbstractPizzaStore *MoscowStore = new MoscowPizzaStore;
    MoscowStore->orderPizza(PizzaTypes::CHEESE);
    MoscowStore->orderPizza(PizzaTypes::BRACCIODIFERRO);

    return 0;
}