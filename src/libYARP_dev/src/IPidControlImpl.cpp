/*
 * Copyright (C) 2006-2018 Istituto Italiano di Tecnologia (IIT)
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms of the
 * BSD-3-Clause license. See the accompanying LICENSE file for details.
 */

#include <yarp/dev/IPidControlImpl.h>
#include <yarp/dev/ControlBoardHelper.h>
#include <yarp/os/LogStream.h>
#include <cmath>

using namespace yarp::dev;
#define JOINTIDCHECK if (j >= castToMapper(helper)->axes()){yError("joint id out of bound"); return false;}
#define MJOINTIDCHECK(i) if (joints[i] >= castToMapper(helper)->axes()){yError("joint id out of bound"); return false;}
#define PJOINTIDCHECK(j) if (j >= castToMapper(helper)->axes()){yError("joint id out of bound"); return false;}

//////////////////// Implement PidControl interface
ImplementPidControl::ImplementPidControl(IPidControlRaw *y)
{
    iPid= dynamic_cast<IPidControlRaw *> (y);
    helper = nullptr;
    temp=nullptr;
    tmpPids=nullptr;
}

ImplementPidControl::~ImplementPidControl()
{
    uninitialize();
}

bool ImplementPidControl:: initialize (int size, const int *amap, const double *enc, const double *zos, const double* newtons, const double* amps, const double* dutys)
{
    if (helper!=nullptr)
        return false;

    helper=(void *)(new ControlBoardHelper(size, amap, enc, zos,newtons,amps,nullptr,dutys));
    yAssert (helper != nullptr);
    temp=new double [size];
    yAssert (temp != nullptr);
    tmpPids=new Pid[size];
    yAssert (tmpPids != nullptr);

    return true;
}

/**
* Clean up internal data and memory.
* @return true if uninitialization is executed, false otherwise.
*/
bool ImplementPidControl::uninitialize ()
{
    if (helper!=nullptr)
        delete castToMapper(helper);
    helper=nullptr;

    checkAndDestroy(tmpPids);
    checkAndDestroy(temp);

     return true;
}

bool ImplementPidControl::setPid(const PidControlTypeEnum& pidtype, int j, const Pid &pid)
{
    JOINTIDCHECK
    Pid pid_machine;
    int k;
    ControlBoardHelper* cb_helper = castToMapper(helper);
    cb_helper->convert_pid_to_machine(pidtype, pid, j, pid_machine, k);
    return iPid->setPidRaw(pidtype, k, pid_machine);
}

bool ImplementPidControl::setPids(const PidControlTypeEnum& pidtype,  const Pid *pids)
{
    int tmp=0;
    ControlBoardHelper* cb_helper = castToMapper(helper);
    int nj= cb_helper->axes();

    for(int j=0;j<nj;j++)
    {
        Pid pid_machine;
        int k;
        cb_helper->convert_pid_to_machine(pidtype,  pids[j], j, pid_machine, k);
        tmpPids[k] = pid_machine;
    }

    return iPid->setPidsRaw(pidtype, tmpPids);
}

bool ImplementPidControl::setPidReference(const PidControlTypeEnum& pidtype,  int j, double ref)
{
    JOINTIDCHECK
    int k=0;
    double raw;
    ControlBoardHelper* cb_helper = castToMapper(helper);
    cb_helper->convert_pidunits_to_machine(pidtype,ref,j,raw,k);
    return iPid->setPidReferenceRaw(pidtype, k, raw);
}

bool ImplementPidControl::setPidReferences(const PidControlTypeEnum& pidtype,  const double *refs)
{
    ControlBoardHelper* cb_helper = castToMapper(helper);
    cb_helper->convert_pidunits_to_machine(pidtype,refs,temp);
    return iPid->setPidReferencesRaw(pidtype, temp);
}

bool ImplementPidControl::setPidErrorLimit(const PidControlTypeEnum& pidtype,  int j, double limit)
{
    JOINTIDCHECK
    int k;
    double raw;
    ControlBoardHelper* cb_helper = castToMapper(helper);
    cb_helper->convert_pidunits_to_machine(pidtype,limit,j,raw,k);
    return iPid->setPidErrorLimitRaw(pidtype, k, raw);
}

bool ImplementPidControl::setPidErrorLimits(const PidControlTypeEnum& pidtype,  const double *limits)
{
    ControlBoardHelper* cb_helper = castToMapper(helper);
    cb_helper->convert_pidunits_to_machine(pidtype,limits,temp);
    return iPid->setPidErrorLimitsRaw(pidtype, temp);
}


bool ImplementPidControl::getPidError(const PidControlTypeEnum& pidtype, int j, double *err)
{
    JOINTIDCHECK
    int k;
    double raw;
    ControlBoardHelper* cb_helper = castToMapper(helper);
    k=castToMapper(helper)->toHw(j);

    bool ret=iPid->getPidErrorRaw(pidtype, k, &raw);

    cb_helper->convert_pidunits_to_user(pidtype,raw,err,k);
    return ret;
}

bool ImplementPidControl::getPidErrors(const PidControlTypeEnum& pidtype,  double *errs)
{
    bool ret;
    ControlBoardHelper* cb_helper = castToMapper(helper);
    ret=iPid->getPidErrorsRaw(pidtype, temp);

    cb_helper->convert_pidunits_to_user(pidtype,temp,errs);
    return ret;
}

bool ImplementPidControl::getPidOutput(const PidControlTypeEnum& pidtype,  int j, double *out)
{
    JOINTIDCHECK
    bool ret;
    int k_raw;
    double raw;
    k_raw = castToMapper(helper)->toHw(j);
    ret = iPid->getPidOutputRaw(pidtype, k_raw, &raw);
    if (ret)
    {
        ControlBoardHelper* cb_helper = castToMapper(helper);
        double output_conversion_units_user2raw = cb_helper->get_pidoutput_conversion_factor_user2raw(pidtype, j);
        *out = raw / output_conversion_units_user2raw;
        return true;
    }
    return false;
}

bool ImplementPidControl::getPidOutputs(const PidControlTypeEnum& pidtype, double *outs)
{
    ControlBoardHelper* cb_helper = castToMapper(helper);
    int nj = cb_helper->axes();
    bool ret = iPid->getPidOutputsRaw(pidtype, temp);
    if (ret)
    {
        castToMapper(cb_helper)->toUser(temp, outs);
        for (int j = 0; j < nj; j++)
        {
            double output_conversion_units_user2raw = cb_helper->get_pidoutput_conversion_factor_user2raw(pidtype, j);
            outs[j] = outs[j] / output_conversion_units_user2raw;
        }
    }
    return ret;
}

bool ImplementPidControl::getPid(const PidControlTypeEnum& pidtype, int j, Pid *pid)
{
    JOINTIDCHECK
    ControlBoardHelper* cb_helper = castToMapper(helper);
    int k_raw;
    k_raw=cb_helper->toHw(j);
    Pid rawPid;
    bool b =iPid->getPidRaw(pidtype, k_raw, &rawPid);
    if (b)
    {
        cb_helper->convert_pid_to_user(pidtype, rawPid, k_raw, *pid, j);
        return true;
    }
    return false;
}

bool ImplementPidControl::getPids(const PidControlTypeEnum& pidtype, Pid *pids)
{
    bool ret=iPid->getPidsRaw(pidtype, tmpPids);
    ControlBoardHelper* cb_helper = castToMapper(helper);
    int nj=cb_helper->axes();

    for (int k_raw = 0; k_raw < nj; k_raw++)
    {
        int j_usr;
        Pid outpid;
        cb_helper->convert_pid_to_user(pidtype, tmpPids[k_raw], k_raw, outpid, j_usr);
        pids[j_usr] = outpid;
    }
    return ret;
}

bool ImplementPidControl::getPidReference(const PidControlTypeEnum& pidtype, int j, double *ref)
{
    JOINTIDCHECK
    bool ret;
    int k;
    double raw;
    ControlBoardHelper* cb_helper = castToMapper(helper);
    k=castToMapper(helper)->toHw(j);

    ret=iPid->getPidReferenceRaw(pidtype, k, &raw);

    cb_helper->convert_pidunits_to_user(pidtype,raw,ref,k);
    return ret;
}

bool ImplementPidControl::getPidReferences(const PidControlTypeEnum& pidtype, double *refs)
{
    bool ret;
    ControlBoardHelper* cb_helper = castToMapper(helper);
    ret=iPid->getPidReferencesRaw(pidtype, temp);

    cb_helper->convert_pidunits_to_user(pidtype,temp,refs);
    return ret;
}

bool ImplementPidControl::getPidErrorLimit(const PidControlTypeEnum& pidtype, int j, double *ref)
{
    JOINTIDCHECK
    bool ret;
    int k;
    double raw;
    ControlBoardHelper* cb_helper = castToMapper(helper);
    k=castToMapper(helper)->toHw(j);

    ret=iPid->getPidErrorLimitRaw(pidtype, k, &raw);

    cb_helper->convert_pidunits_to_user(pidtype,raw,ref,k);
    return ret;
}

bool ImplementPidControl::getPidErrorLimits(const PidControlTypeEnum& pidtype, double *refs)
{
    bool ret;
    ControlBoardHelper* cb_helper = castToMapper(helper);
    ret=iPid->getPidErrorLimitsRaw(pidtype, temp);

    cb_helper->convert_pidunits_to_user(pidtype,temp,refs);
    return ret;
}

bool ImplementPidControl::resetPid(const PidControlTypeEnum& pidtype, int j)
{
    JOINTIDCHECK
    int k=0;
    k=castToMapper(helper)->toHw(j);

    return iPid->resetPidRaw(pidtype, k);
}

bool ImplementPidControl::enablePid(const PidControlTypeEnum& pidtype, int j)
{
    JOINTIDCHECK
    int k=0;
    k=castToMapper(helper)->toHw(j);

    return iPid->enablePidRaw(pidtype, k);
}

bool ImplementPidControl::disablePid(const PidControlTypeEnum& pidtype, int j)
{
    JOINTIDCHECK
    int k=0;
    k=castToMapper(helper)->toHw(j);

    return iPid->disablePidRaw(pidtype, k);
}

bool ImplementPidControl::setPidOffset(const PidControlTypeEnum& pidtype, int j, double off)
{
    JOINTIDCHECK
    int k = 0;
    double rawoff;
    ControlBoardHelper* cb_helper = castToMapper(helper);
    double output_conversion_units_user2raw = cb_helper->get_pidoutput_conversion_factor_user2raw(pidtype,j);
    rawoff = off * output_conversion_units_user2raw;
    return iPid->setPidOffsetRaw(pidtype, k, rawoff);
}

bool ImplementPidControl::isPidEnabled(const PidControlTypeEnum& pidtype, int j, bool* enabled)
{
    JOINTIDCHECK
    int k=0;
    k=castToMapper(helper)->toHw(j);

    return iPid->isPidEnabledRaw(pidtype, k, enabled);
}

bool ImplementPidControl::setConversionUnits(const PidControlTypeEnum& pidtype, const PidFeedbackUnitsEnum fbk_conv_units, const PidOutputUnitsEnum out_conv_units)
{
    castToMapper(helper)->set_pid_conversion_units(pidtype, fbk_conv_units, out_conv_units);
    return true;
}
