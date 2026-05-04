// SymbolicLink.cpp — Symbolic link implementation
// Faraz Aamir | 25I-2069 | OOP Project | Spring 2026

#include "SymbolicLink.h"

SymbolicLink::SymbolicLink(const MyString& name, User* owner,
                           FileSystemEntity* target, const Permission& perms)
    : FileSystemEntity(name, owner, perms), target(target) {}

SymbolicLink::~SymbolicLink() {
    // Does NOT delete target (aggregation)
}

size_t SymbolicLink::getSize() const { return 0; }

MyString SymbolicLink::getType() const { return MyString("SymLink"); }

FileSystemEntity* SymbolicLink::clone(User* newOwner) const {
    SymbolicLink* copy = new SymbolicLink(name, newOwner, target, permissions);
    for (int i = 0; i < extendedAttributes.size(); i++) {
        copy->addAttribute(extendedAttributes[i].getKey(), extendedAttributes[i].getValue());
    }
    return copy;
}

FileSystemEntity* SymbolicLink::resolve() const {
    if (isDangling()) {
        std::cout << "  ERROR: Symbolic link '" << name << "' is dangling." << std::endl;
        return nullptr;
    }
    FileSystemEntity* current = target;
    int maxDepth = 20;
    while (current && current->getType() == "SymLink" && maxDepth > 0) {
        current = dynamic_cast<SymbolicLink*>(current)->target;
        maxDepth--;
    }
    if (maxDepth == 0) {
        std::cout << "  ERROR: Symbolic link loop detected for '" << name << "'." << std::endl;
        return nullptr;
    }
    return current;
}

bool SymbolicLink::isDangling() const { return target == nullptr; }
FileSystemEntity* SymbolicLink::getTarget() const { return target; }
void SymbolicLink::setTarget(FileSystemEntity* newTarget) { target = newTarget; }

void SymbolicLink::display(std::ostream& os, int indent) const {
    for (int i = 0; i < indent * 2; i++) os << ' ';
    os << permissions << " " << owner->getUsername() << " 0\t" << name << " -> ";
    if (isDangling()) os << "[DANGLING]";
    else os << target->getName();
    os << std::endl;
}
