#pragma once

class Stmt {
public:
    virtual ~Stmt() = default;
    virtual void print() const = 0;
};
