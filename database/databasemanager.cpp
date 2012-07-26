#include "databasemanager.h"

namespace Database
{
    DatabaseManager::DatabaseManager()
    {
        database = QSqlDatabase::database();
        database.open();
    }

    DatabaseManager::~DatabaseManager()
    {
        database.close();
    }

    std::vector<Category> DatabaseManager::getCategories()
    {
        std::vector<Category> categories;

        QSqlQuery query("SELECT * FROM categories");
        while(query.next()) {
            int id = query.value(0).toInt();
            QString arabicName = query.value(1).toString();
            QString englishName = query.value(2).toString();

            Category category(id, arabicName, englishName);
            categories.push_back(category);
        }

        return categories;
    }

    std::vector<Item> DatabaseManager::getItemsInCategory(int categoryId)
    {
        std::vector<Item> items;
        QSqlQuery query(QString("SELECT * FROM items WHERE categories_id = %1").arg(categoryId));

        while(query.next()) {
            int id = query.value(0).toInt();
            QString arabicName = query.value(1).toString();
            QString englishName = query.value(2).toString();
            int categoryId = query.value(3).toInt();

            Item item(id, arabicName, englishName, categoryId);
            items.push_back(item);
        }

        return items;
    }

    std::vector<ItemDetail> DatabaseManager::getItemDetails(int itemId)
    {
        std::vector<ItemDetail> itemDetails;

        QSqlQuery query(QString("SELECT * FROM item_details WHERE item_id = %1").arg(itemId));
        while(query.next()) {
            int id = query.value(0).toInt();
            int itemId = query.value(1).toInt();
            int sizeId = query.value(2).toInt();
            int price = query.value(3).toInt();

            ItemDetail itemDetail = ItemDetail(id, itemId, sizeId, price);
            itemDetails.push_back(itemDetail);
        }

        return itemDetails;
    }

    ItemDetail DatabaseManager::getItemDetailById(int itemDetialId)
    {
        ItemDetail itemDetail;

        QSqlQuery query(QString("SELECT * FROM item_details WHERE id = %1").arg(itemDetialId));
        while(query.next()) {
            int id = query.value(0).toInt();
            int itemId = query.value(1).toInt();
            int sizeId = query.value(2).toInt();
            int price = query.value(3).toInt();

            itemDetail = ItemDetail(id, itemId, sizeId, price);
            break;
        }

        return itemDetail;
    }

    Item DatabaseManager::getItemById(int itemId) {
        Item item;

        QSqlQuery query(QString("SELECT * FROM items WHERE id = %1").arg(itemId));
        while(query.next()) {
            int id = query.value(0).toInt();
            QString arabicName = query.value(1).toString();
            QString englishName = query.value(2).toString();
            int categoryId = query.value(3).toInt();

            item = Item(id, arabicName, englishName, categoryId);
        }

        return item;
    }

    std::vector<Component> DatabaseManager::getAllCompnents()
    {
        std::vector<Component> components;

        QSqlQuery query(QString("SELECT * FROM components"));
        while(query.next()){
            int id = query.value(0).toInt();
            QString arabicName = query.value(1).toString();
            QString englishName = query.value(2).toString();

            Component component(id, arabicName, englishName);
            components.push_back(component);
        }

        return components;
    }

    std::vector<Component> DatabaseManager::getCompnentsInItem(int itemId)
    {
        std::vector<Component> components;

        QSqlQuery query(QString("SELECT * FROM item_components WHERE item_id = %1").arg(itemId));
        while(query.next()){
            int componentId = query.value(2).toInt();
            components.push_back(getComponentById(componentId));
        }

        return components;
    }

    Component DatabaseManager::getComponentById(int componentId)
    {
        Component component;

        QSqlQuery query(QString("SELECT * FROM components WHERE id = %1").arg(componentId));
        while(query.next()) {
            int id = query.value(0).toInt();
            QString arabicName = query.value(1).toString();
            QString englishName = query.value(2).toString();

            component = Component(id, arabicName, englishName);
        }

        return component;
    }

    Additionals DatabaseManager::getAdditionalsById(int additionalsId)
    {
        Additionals additionals;

        QSqlQuery query(QString("SELECT * FROM additionals WHERE id = %1").arg(additionalsId));
        while(query.next()) {
            int id = query.value(0).toInt();
            QString arabicName = query.value(1).toString();
            QString englishName = query.value(2).toString();
            int price = query.value(3).toInt();

            additionals = Additionals(id, arabicName, englishName, price);
            break;
        }

        return additionals;
    }

    std::vector<Additionals> DatabaseManager::getAllAdditionals()
    {
        std::vector<Additionals> additionals;

        QSqlQuery query(QString("SELECT * FROM additionals"));
        while(query.next()){
            int id = query.value(0).toInt();
            QString arabicName = query.value(1).toString();
            QString englishName = query.value(2).toString();
            int price = query.value(3).toInt();

            Additionals additional(id, arabicName, englishName, price);
            additionals.push_back(additional);
        }

        return additionals;
    }

    QString DatabaseManager::getItemSizeDescription(int sizeId, LAGNUAGE language) {
        QString result = "";

        QSqlQuery query(QString("SELECT * FROM item_size WHERE id = %1").arg(sizeId));
        while(query.next()) {
            //int id = query.value(0).toInt();
            QString arabicName = query.value(1).toString();
            QString englishName = query.value(2).toString();

            if ( language == ARABIC ) {
                result = arabicName;
            }
            else {
                result = englishName;
            }
        }

        return result;
    }

    bool DatabaseManager::addOrder(QDateTime currentTime, int orderTypeId, int cash,
                                   int discount, int totalCash, QList<Model::Order> orders) {

        QSqlQuery query;

        query.prepare("INSERT INTO orders(order_time, order_type_id, cash, discount, total_cash, is_cancelled) VALUES(?,?,?,?,?,0)");
        query.addBindValue(currentTime);
        query.addBindValue(orderTypeId);
        query.addBindValue(cash);
        query.addBindValue(discount);
        query.addBindValue(totalCash);

        bool ret = query.exec();

        if ( ret ) {
            int orderId = query.lastInsertId().toInt();

            foreach(Model::Order order, orders) {

                QSqlQuery tmpQuery;

                tmpQuery.prepare("INSERT INTO order_details(id, order_id, item_detial_id, quantity, components_ids, additionals_ids, sugar, cash) VALUES(NULL, ?, ?, ?, ?, ?, ?, ?)");
                tmpQuery.addBindValue(orderId);
                tmpQuery.addBindValue(order.getItemDetialId());
                tmpQuery.addBindValue(order.getQunatity());
                tmpQuery.addBindValue(fromListToText(order.getComponentsIds()));
                tmpQuery.addBindValue(fromListToText(order.getAdditionalsIds()));
                tmpQuery.addBindValue(order.getSugar());
                tmpQuery.addBindValue(order.getCash());

                qDebug() << " Insert Order: " << order.getItemDetialId() << " : " << tmpQuery.exec();
            }
        }

        return ret;
    }

    bool DatabaseManager::cancelOrder(int orderId) {
        QSqlQuery query;

        query.prepare("UPDATE orders SET is_cancelled = 1 WHERE id = ?");
        query.addBindValue(orderId);

        bool ret = query.exec();

        if ( ret ) {
            query.prepare("INSERT INTO order_canceled(order_id, cancel_time) VALUES(?,?)");
            query.addBindValue(orderId);
            query.addBindValue(QDateTime::currentDateTime());

            ret = query.exec();
        }

        return ret;
    }

    bool DatabaseManager::closeTodayOrders(QDateTime time) {
        QSqlQuery query;

        query.prepare("INSERT INTO system_checkout(checkout_time) VALUES(?)");
        query.addBindValue(time);

        return query.exec();
    }

    bool DatabaseManager::addLoginEventLogging(int userId, QDateTime time, int eventType) { // 1 login, 2 logout
        QSqlQuery query;

        query.prepare("INSERT INTO events_logging(user_id, event_time, event_type) VALUES(?,?,?)");
        query.addBindValue(userId);
        query.addBindValue(time);
        query.addBindValue(eventType);

        return query.exec();
    }

    QString DatabaseManager::fromListToText(QStringList ids) {
        QString commaSeparatedId = "";

        for(int i=0; i<ids.size(); i++) {
            commaSeparatedId += ids.at(i);

            if ( i < ids.size() - 1 ) {
                commaSeparatedId += ",";
            }
        }

        return commaSeparatedId;
    }

    QStringList DatabaseManager::fromTextToList(QString text) {
        return text.split(",");
    }

    User DatabaseManager::getUserById(int id) {
        User user ;

        QSqlQuery query(QString("SELECT * FROM users WHERE id = ?"));
        query.addBindValue(id);

        while(query.next()){
            int id = query.value(0).toInt();
            QString username = query.value(1).toString();
            QString password = query.value(2).toString();
            QDateTime registerTime = query.value(3).toDateTime();

            user = User(id, username, password, registerTime);
            break;
        }

        return user;
    }

    QList<LoginReport> DatabaseManager::getLoginReport(QDateTime from, QDateTime to) {
        QList<LoginReport> logins;

        QSqlQuery query(QString("SELECT * FROM events_logging"));
        query.addBindValue(from);
        query.addBindValue(to);

        while(query.next()){
            int id = query.value(0).toInt();
            User user = getUserById(query.value(1).toInt());
            QDateTime eventTime = query.value(2).toDateTime();
            int eventType = query.value(3).toInt();

            LoginReport login(id, user, eventTime, eventType);
            logins.append(login);
        }

        return logins;
    }
}
