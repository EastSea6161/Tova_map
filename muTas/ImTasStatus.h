#pragma once

class KTarget;

class KImTasStatus
{
public:
    KImTasStatus ();
    ~KImTasStatus();
public:
    static bool IsRunAssign(KTarget* a_pTarget);
};