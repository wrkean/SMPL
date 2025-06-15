#pragma once

class StmtNode {
public:
    virtual ~StmtNode() = default;
    virtual void print() const = 0;
};
