#pragma once
#include "parser.h"
#include "HTMLElement.h"
#include <vector>

using HTMLTokenIterator = std::vector<HTMLToken>::const_iterator;

void assemble_dom(HTMLElement *parent, HTMLTokenIterator start, HTMLTokenIterator end);
HTMLElement *create_dom(const std::vector<HTMLToken> &tokens);