#ifndef SIMULATOR_INTERFACE_HPP
#define SIMULATOR_INTERFACE_HPP

namespace simctrl
{
template <typename CmdDataType, typename StateDataType, typename SensorDataType>
class SimInterface
{
public:
    virtual void SetInitialState(const StateDataType &init) = 0;
    virtual bool Receive(const CmdDataType &cmd) = 0;
    void Send(SensorDataType &data)
    {
        Update();
        ConvertStateToSensor(SensorDataType &data);
    }
protected:
    SimInterface(){};
    virtual ~SimInterface() = default;
    virtual void Update() = 0;
    virtual void ConvertStateToSensor(SensorDataType &data) = 0;
    bool initialized = false;
};
}


#endif