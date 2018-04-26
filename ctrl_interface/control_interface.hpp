#ifndef CONTROL_INTERFACE_HPP
#define CONTROL_INTERFACE_HPP

namespace simctrl
{
template <typename SensorDataType, typename CmdDataType>
class CtrlInterface
{
public:
    virtual void SetCtrlSysParam() = 0;
    virtual bool Receive(const SensorDataType &data) = 0;
    virtual void Send(CmdDataType &cmd) = 0;
protected:
    CtrlSysInterface(){};
    virtual ~CtrlSysInterface() = default;
    virtual void Update() = 0;
};
}


#endif