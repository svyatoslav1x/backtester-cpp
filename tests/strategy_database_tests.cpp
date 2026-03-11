#include "strategy_database_tests.h"

#include <QSqlQuery>

#include "StrategyDatabase.h"

TEST_F(StrategyDatabaseTest, InitializeCreatesOpenDatabase) {
    EXPECT_TRUE(StrategyDatabase::initialize());

    const auto db = StrategyDatabase::database();
    EXPECT_TRUE(db.isOpen());
}

TEST_F(StrategyDatabaseTest, TablesExistAfterInitialize) {
    ASSERT_TRUE(StrategyDatabase::initialize());

    const auto db = StrategyDatabase::database();
    ASSERT_TRUE(db.isOpen());

    QSqlQuery query(db);

    ASSERT_TRUE(query.exec(
        "SELECT name FROM sqlite_master "
        "WHERE type='table' AND name='strategies'"
    ));
    ASSERT_TRUE(query.next());
    EXPECT_EQ(query.value(0).toString(), "strategies");

    ASSERT_TRUE(query.exec(
        "SELECT name FROM sqlite_master "
        "WHERE type='table' AND name='strategy_parameters'"
    ));
    ASSERT_TRUE(query.next());
    EXPECT_EQ(query.value(0).toString(), "strategy_parameters");
}
