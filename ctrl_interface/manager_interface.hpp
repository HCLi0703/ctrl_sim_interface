#ifndef PROCESS_INTERFACE_HPP
#define PROCESS_INTERFACE_HPP

#include <memory>
#include <iostream>
#include <cstdint>

#include "control_interface.hpp"
#include "simulator_interface.hpp"

namespace simctrl
{
template <typename CmdDataType, typename StateDataType, typename SensorDataType>
class ProcessInterface
{
public:
    ProcessInterface(std::shared_ptr<CtrlInterface<SensorDataType, CmdDataType> > control_sys, 
    std::shared_ptr<SimInterface<CmdDataType, StateDataType, SensorDataType> > simulator):
    sim(simulator), ctrl(control_syst), sim_ctrl_ready_(false), loop_count_(0){};

    virtual ~ProcessInterface() = default;

    void Setup(const double &sim_time_, const double &freq, const StateDataType &init)
    {
        SimCtrlReady();

        if(!sim_ctrl_ready_)
        return;
        else{
            SetupSimTime(sim_time_, freq);
        }

        sim -> SetInitialState(init);
    }

    void Start(){

        SimCtrlReady();
        
        while(sim_ctrl_ready_ && loop_count_ <= max_count_)
        {
            if(loop_count_ == 0)
            {
                std::cout << "Enter Simulation Loop. Total Simulation Time is " << sim_time_ << " Second(s), Time Step is " 
                << step << " Second, Good Luck!!!" << std::endl;         
            }

            sim -> Send(_data);

            if(!ctrl -> Receive(_data))
            {
                std::cout << "INFO: Fail to Receive Data From Sensor." << std::endl;
            }

            ctrl -> Send(_cmd);

            sim -> Receive(_cmd);

            loop_count_++;
        }

        std::cout << "INFO: Exited Simulation Loop." << std::endl;
    }

private:
    CmdDataType _cmd;
    SensorDataType _data;
    StateDataType _state;

    std::shared_ptr<CtrlInterface<SensorDataType, CmdDataType> > sim;
    std::shared_ptr<SimInterface<CmdDataType, SensorDataType> > ctrl;

    double sim_time_;
    double step;
    uint64_t max_count_;
	uint64_t loop_count_;
    bool sim_ctrl_ready_;

    void SimCtrlReady()
    {
        if(ctrl != nullptr && sim != nullptr)
        sim_ctrl_ready_ = true;
        else if (ctrl == nullptr){
        sim_ctrl_ready_ = false;
        std::cout << "Please Choose a Control System !!" << std:: endl;
        }
        else{
            sim_ctrl_ready_ = false;
            std::cout << "Please Choose a Simulator !!" << std::endl;
        }
    }

    void SetupSimTime(const double total_time, const double freq)
    {
        sim_time_ = total_time;
        step = 1.0 / freq;
        max_count_ = (uint64_t) sim_time_ / step;
    }
};
}


#endif