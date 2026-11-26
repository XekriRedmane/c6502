// The leading `.` of a renamed label makes it a dasm local label, valid
// only up to the next `SUBROUTINE` directive.
#include "passes/label_resolution.h"

#include <string>
#include <unordered_map>
#include <utility>

#include "errors.h"
#include "pyrepr.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::c99_ast;

using Labels = std::unordered_map<std::string, std::string>;

[[noreturn]] void fail(const std::string& message) {
  throw CompileError("LabelResolutionError", message);
}

void collect_statement(const ir::Type_statement& stmt, const std::string& fn_name,
                       Labels& labels);

void collect_block(const ir::Type_block& block, const std::string& fn_name, Labels& labels) {
  for (const auto& item : std::get<ir::Block>(block).block_item) {
    if (const auto* s = std::get_if<ir::S>(&item)) {
      collect_statement(*s->statement, fn_name, labels);
    }
  }
}

void collect_statement(const ir::Type_statement& stmt, const std::string& fn_name,
                       Labels& labels) {
  switch (ir::kind(stmt)) {
    case ir::Kind_statement::LabeledStmt: {
      const auto& l = std::get<ir::LabeledStmt>(stmt);
      if (labels.count(l.label) != 0) {
        fail("duplicate label " + pyrepr::string(l.label));
      }
      labels[l.label] = "." + fn_name + "@" + l.label;
      collect_statement(*l.statement, fn_name, labels);
      return;
    }
    case ir::Kind_statement::IfStmt: {
      const auto& i = std::get<ir::IfStmt>(stmt);
      collect_statement(*i.then_clause, fn_name, labels);
      if (i.else_clause) {
        collect_statement(**i.else_clause, fn_name, labels);
      }
      return;
    }
    case ir::Kind_statement::Compound:
      collect_block(*std::get<ir::Compound>(stmt).block, fn_name, labels);
      return;
    case ir::Kind_statement::WhileStmt:
      collect_statement(*std::get<ir::WhileStmt>(stmt).body, fn_name, labels);
      return;
    case ir::Kind_statement::DoWhileStmt:
      collect_statement(*std::get<ir::DoWhileStmt>(stmt).body, fn_name, labels);
      return;
    case ir::Kind_statement::ForStmt:
      collect_statement(*std::get<ir::ForStmt>(stmt).body, fn_name, labels);
      return;
    case ir::Kind_statement::SwitchStmt:
      collect_statement(*std::get<ir::SwitchStmt>(stmt).body, fn_name, labels);
      return;
    case ir::Kind_statement::CaseStmt:
      collect_statement(*std::get<ir::CaseStmt>(stmt).body, fn_name, labels);
      return;
    case ir::Kind_statement::DefaultStmt:
      collect_statement(*std::get<ir::DefaultStmt>(stmt).body, fn_name, labels);
      return;
    default:
      return;
  }
}

void rewrite_statement(ir::Type_statement& stmt, const Labels& labels);

void rewrite_block(ir::Type_block& block, const Labels& labels) {
  for (auto& item : std::get<ir::Block>(block).block_item) {
    if (auto* s = std::get_if<ir::S>(&item)) {
      rewrite_statement(*s->statement, labels);
    }
  }
}

void rewrite_statement(ir::Type_statement& stmt, const Labels& labels) {
  switch (ir::kind(stmt)) {
    case ir::Kind_statement::Goto: {
      auto& g = std::get<ir::Goto>(stmt);
      auto it = labels.find(g.label);
      if (it == labels.end()) {
        fail("goto to undefined label " + pyrepr::string(g.label));
      }
      g.label = it->second;
      return;
    }
    case ir::Kind_statement::LabeledStmt: {
      auto& l = std::get<ir::LabeledStmt>(stmt);
      l.label = labels.at(l.label);
      rewrite_statement(*l.statement, labels);
      return;
    }
    case ir::Kind_statement::IfStmt: {
      auto& i = std::get<ir::IfStmt>(stmt);
      rewrite_statement(*i.then_clause, labels);
      if (i.else_clause) {
        rewrite_statement(**i.else_clause, labels);
      }
      return;
    }
    case ir::Kind_statement::Compound:
      rewrite_block(*std::get<ir::Compound>(stmt).block, labels);
      return;
    case ir::Kind_statement::WhileStmt:
      rewrite_statement(*std::get<ir::WhileStmt>(stmt).body, labels);
      return;
    case ir::Kind_statement::DoWhileStmt:
      rewrite_statement(*std::get<ir::DoWhileStmt>(stmt).body, labels);
      return;
    case ir::Kind_statement::ForStmt:
      rewrite_statement(*std::get<ir::ForStmt>(stmt).body, labels);
      return;
    case ir::Kind_statement::SwitchStmt:
      rewrite_statement(*std::get<ir::SwitchStmt>(stmt).body, labels);
      return;
    case ir::Kind_statement::CaseStmt:
      rewrite_statement(*std::get<ir::CaseStmt>(stmt).body, labels);
      return;
    case ir::Kind_statement::DefaultStmt:
      rewrite_statement(*std::get<ir::DefaultStmt>(stmt).body, labels);
      return;
    default:
      return;
  }
}

}  // namespace

c99_ast::Program resolve_labels(c99_ast::Program prog) {
  for (auto& decl : prog.declaration) {
    auto* fd = std::get_if<ir::FunctionDecl>(&decl);
    if (fd == nullptr || !fd->function_decl->body) {
      continue;
    }
    auto& f = *fd->function_decl;
    Labels labels;
    collect_block(**f.body, f.name, labels);
    rewrite_block(**f.body, labels);
  }
  return prog;
}

}  // namespace c6502::passes
