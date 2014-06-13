/**
 * \file systemled.hpp
 * \author Thibault Schueller <ryp.sqrt@gmail.com>
 * \brief SystemLed class declaration
 */

#ifndef SYSTEMLED_HPP
#define SYSTEMLED_HPP

#include <cstdint>

#include "hardware/iserializabledevice.hpp"

class SystemLed : public ISerializableDevice
{
    static const std::string    DevicePathPrefix;

public:
    explicit SystemLed(const std::string& name);
    ~SystemLed() = default;

    SystemLed(const SystemLed& other) = delete;
    SystemLed& operator=(const SystemLed& other) = delete;

public:
    virtual const std::string&  getName() const override;
    virtual void                deserialize(const ptree& node) override;
    virtual void                serialize(ptree& node) override;

public:
    void            blink();
    int             getBrightness() const;
    void            setBrightness(int value);
    std::string     getTrigger() const;
    void            setTrigger(const std::string& trigger);
    int             getDelayOn() const;
    void            setDelayOn(int delay);
    int             getDelayOff() const;
    void            setDelayOff(int delay);

private:
    void    test();

private:
    const std::string   _name;
    std::string         _ledName;
    std::string         _path;
    std::string         _brightnessFile;
    std::string         _triggerFile;
    std::string         _delayOnFile;
    std::string         _delayOffFile;
};

#endif // SYSTEMLED_HPP
