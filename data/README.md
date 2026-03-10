## Database

`data/database/strategies.db`
### Tables

#### `strategies`
Stores one row per strategy.

Fields:
- `id` – unique strategy ID
- `name` – unique strategy name
- `model_type` – strategy type
- `is_editable` – determines if the whole strategy can be edited

#### `strategy_parameters`
Stores parameters belonging to a strategy.

Fields:
- `id` – unique parameter row ID
- `strategy_id` – references the strategy in `strategies`
- `param_key` – parameter name
- `param_value` – parameter value
- `value_type` – parameter type (`int`, `double`, etc.)
- `is_editable` – determines if this specific parameter can be edited

### P.S
- Strategy names must be unique.
- A strategy can have different parameters depending on its model type.