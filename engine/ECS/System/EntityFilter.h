#ifndef ENTITY_FILTER_H
#define ENTITY_FILTER_H

#include <set>
#include <map>
#include <string>
#include <typeindex>

#include "Component.h"
#include "Entity.h"

// ----------------------------------------------------------------------------
// EntityFilter
//
// This class allows one to filter Entities based on the types of components
// that they have.
// ----------------------------------------------------------------------------
class EntityFilter {
public:
   // --------------------------------------------------------------------------
   // Expression
   //
   // This is a base class for ExpressionTuples to allow for polymorphic usage
   // usage.
   // --------------------------------------------------------------------------
   class Expression {
   public:
      Expression(bool in_result) : in_result_(in_result) {}
      virtual ~Expression() {}

      // public interface
      virtual bool filter(Entity& e) = 0;

   protected:
      bool in_result() const {
         return this->in_result_;
      }

   private:
      bool in_result_;
   };

   // --------------------------------------------------------------------------
   // ExpressionTuple
   //
   // This is a base class for a simple boolean expression of Component Types.
   // The Entity Filter is the combined logical AND of the filter results of
   // all ExpressionTuples registered to it.
   // --------------------------------------------------------------------------
   template <typename ...Tokens>
   class ExpressionTuple : public Expression {
   public:
      ExpressionTuple<Tokens...>(bool in_result) : Expression(in_result) {}
      virtual ~ExpressionTuple<Tokens...>() {}

      virtual bool filter(Entity& e) {
         return this->traverse_tuple<std::tuple_size<std::tuple<Tokens...>>::value>(e, this->in_result());
      }

   private:
      // child interface
      virtual bool evaluate(Entity& e, bool has_token, bool in_result) = 0;

      // private helper methods for folding tuples
      template <size_t N>
      using TokenType = typename std::tuple_element<N, std::tuple<Tokens...>>::type;

      template <size_t N, typename std::enable_if<(N > 1)>::type* = nullptr>
      bool traverse_tuple(Entity& e, bool in_result) {
         return this->traverse_tuple<N - 1>(e, evaluate(e, e.get<TokenType<N - 1>>() != nullptr, in_result));
      }

      template <size_t N, typename std::enable_if<(N == 1)>::type* = nullptr>
      bool traverse_tuple(Entity& e, bool in_result) {
         return evaluate(e, e.get<TokenType<N - 1>>() != nullptr, in_result);
      }
   };

   // --------------------------------------------------------------------------
   // And
   //
   // This is a boolean AND of all involved tokens. Filter will only return
   // true if the filtered entity has ALL components of the expression.
   // --------------------------------------------------------------------------
   template <typename ...Tokens>
   class And : public ExpressionTuple<Tokens...> {
   public:
      And() : ExpressionTuple<Tokens...>(true) {}

      virtual bool evaluate(Entity& e, bool has_token, bool in_result) {
         return has_token && in_result;
      }
   };

   // --------------------------------------------------------------------------
   // Or
   //
   // This is a boolean OR of all involved tokens. Filter will only return
   // true if the filtered entity has AT LEAST one of the specified
   // components.
   // --------------------------------------------------------------------------
   template <typename ...Tokens>
   class Or : public ExpressionTuple<Tokens...> {
   public:
      Or() : ExpressionTuple<Tokens...>(false) {}
   
      virtual bool evaluate(Entity& e, bool has_token, bool in_result) {
         return has_token || in_result;
      }
   };

   // --------------------------------------------------------------------------
   // Not
   //
   // This is a boolean NOT of all involved tokens. The filter will only
   // return true if the filtered entity does not have ANY of the specified
   // Components.
   // --------------------------------------------------------------------------
   template <typename ...Tokens>
   class Not : public ExpressionTuple<Tokens...> {
   public:
      Not() : ExpressionTuple<Tokens...>(true) {}
   
      virtual bool evaluate(Entity& e, bool has_token, bool in_result) {
         return !has_token && in_result;
      }
   };

   // --------------------------------------------------------------------------
   // Entity Filter body
   // --------------------------------------------------------------------------
   EntityFilter(const std::string& id = "EntityFilter");
   ~EntityFilter();

   const std::string& id() const;
   void id(const std::string& id);

   bool allow_all() const {
      return this->allow_all();
   }

   void allow_all(bool allow_policy) {
      this->allow_all_ = allow_policy;
   }

   // public interface to add expressions
   template <typename ...Components>
   EntityFilter& all_of() {
      this->all_of_.insert(new And<Components...>());
      return *this;
   }

   template <typename ...Components>
   EntityFilter& one_of() {
      this->one_of_.insert(new Or<Components...>());
      return *this;
   }
   
   template <typename ...Components>
   EntityFilter& none_of() {
      this->none_of_.insert(new Not<Components...>());
      return *this;
   }

   // public interface for filtering
   bool filter(Entity& e) const;

private:
   std::string id_;

   bool allow_all_; // if this is true, filter returns true no matter what

   std::set<Expression*> all_of_;
   std::set<Expression*> one_of_;
   std::set<Expression*> none_of_;
};

#endif
