# ClauScript

# Goal
UserType Traversing -> Expression Traversing? <- experimental?

```cpp
class Event {
 public:
  std::string id;
  std::ArrayMap<std::string, DataType> local;
  std::ArrayMap<std::string, DataType> parameter;
  std::vector<Statement*> statements;
};

class Statement { };

class If : public Statement {
  public:
    Expression* condition;
    std::vector<Statement*> statements; // executed if conidtion is true
};
```
# ClauScript Code
    Event = 
    {
      id = replace

      $local = { name value }

      $parameter = { name value is_user_type  }	


      $if = { 
        condition = { 
          $AND_ALL = { $EQ = { FALSE $parameter.is_user_type } 
            $COMP> = { $parameter.name 1444.1.1 }
            $NOT = { $is_quoted_str = { $parameter.name } }
          }
        } 
        then = {
          $assign = { $local.name data = { 1444.1.1 } }
          $assign = { $local.value data = { $parameter.value } }

          $return = { name = $local.name value = $local.value }
        }
      }


      $if = { 
        condition = { 
          $AND_ALL = { $EQ = { FALSE $parameter.is_user_type } 
            $COMP> = { $remove_quoted = { $parameter.name } 1444.1.1 }
            $is_quoted_str = { $parameter.name }
          }
        } 
        then = {
          $assign = { $local.name data = { "1444.1.1" } }
          $assign = { $local.value data = { $parameter.value } }

          $return = { name = $local.name value = $local.value }
        }
      }

      $if = { 
        condition = { 
          $AND_ALL = { $EQ = { FALSE $parameter.is_user_type } 
            $COMP> = { $parameter.value 1444.1.1 }
            $NOT = { $is_quoted_str = { $parameter.value } }
          }
        }                               
        then = {
          $assign = { $local.name data = { $parameter.name } }
          $assign = { $local.value data = { 1444.1.1 } }

          $return = { name = $local.name value = $local.value }
        }
      }

      $if = { 
        condition = { 
          $AND_ALL = { $EQ = { FALSE $parameter.is_user_type } 
            $COMP> = { $remove_quoted = { $parameter.value } 1444.1.1 }
            $is_quoted_str = { $parameter.value }
          }
        } 
        then = {
          $assign = { $local.name data = { $parameter.name } }
          $assign = { $local.value data = { "1444.1.1" } }

          $return = { name = $local.name value = $local.value }
        }
      }

      $if = { 
        condition = { 
          $AND_ALL = { $EQ = { TRUE $parameter.is_user_type } 
            $COMP> = { $parameter.name 1444.1.1 }
            $NOT = { $is_quoted_str = { $parameter.name } }
          }
        } 
        then = {
          $assign = { $local.name data = { 1444.1.1 } }

          $return = { name = $local.name }
        }
      }

      $if = { 
        condition = { 
          $AND_ALL = { $EQ = { TRUE $parameter.is_user_type } 
            $COMP> = { $remove_quoted = { $parameter.name } 1444.1.1 }
            $is_quoted_str = { $parameter.name }
          }
        } 
        then = {
          $assign = { $local.name data = { "1444.1.1" } }

          $return = { name = $local.name }
        }
      }
    }
