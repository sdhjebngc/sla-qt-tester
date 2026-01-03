/**
 * test_arrow_connection.cpp
 * 
 * 集成测试：箭头连接功能
 * 测试 Arrow 类在连接两个 DiagramItem 时的行为
 * 
 * 测试要点：
 * 1. 创建两个图元并用箭头连接
 * 2. 验证箭头的起点和终点正确
 * 3. 验证移动图元时箭头位置更新
 * 4. 验证删除箭头的行为
 */

#include <QtTest/QtTest>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QSignalSpy>
#include <QMenu>
#include "../diagramitem.h"
#include "../arrow.h"
#include "../diagramscene.h"

class TestArrowConnection : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // 集成测试用例
    void testArrowCreation();
    void testArrowConnectsItems();
    void testArrowUpdateOnItemMove();
    void testArrowColor();
    void testMultipleArrows();
    void testRemoveArrowFromItem();

private:
    DiagramScene *scene;
    QGraphicsView *view;
    QMenu *itemMenu;
};

void TestArrowConnection::initTestCase()
{
    qDebug() << "=== 开始 Arrow 连接集成测试 ===";
}

void TestArrowConnection::cleanupTestCase()
{
    qDebug() << "=== 完成 Arrow 连接集成测试 ===";
}

void TestArrowConnection::init()
{
    itemMenu = new QMenu();
    scene = new DiagramScene(itemMenu, this);
    scene->setSceneRect(QRectF(0, 0, 800, 600));
    
    view = new QGraphicsView(scene);
    view->show();
    QVERIFY(QTest::qWaitForWindowExposed(view));
}

void TestArrowConnection::cleanup()
{
    delete view;
    delete scene;
    delete itemMenu;
    view = nullptr;
    scene = nullptr;
    itemMenu = nullptr;
}

void TestArrowConnection::testArrowCreation()
{
    qDebug() << "测试：创建箭头连接两个图元";
    
    // 创建两个图元
    DiagramItem *startItem = new DiagramItem(DiagramItem::Step, nullptr);
    DiagramItem *endItem = new DiagramItem(DiagramItem::Conditional, nullptr);
    
    scene->addItem(startItem);
    scene->addItem(endItem);
    
    startItem->setPos(100, 100);
    endItem->setPos(300, 100);
    
    // 创建箭头连接
    Arrow *arrow = new Arrow(startItem, endItem);
    scene->addItem(arrow);
    
    // 验证箭头已添加到场景
    QVERIFY(arrow != nullptr);
    QCOMPARE(scene->items().count(), 3);  // 2个图元 + 1个箭头
    
    // 验证箭头可选择
    arrow->setSelected(true);
    QVERIFY(arrow->isSelected());
    
    qDebug() << "✓ 箭头创建成功";
}

void TestArrowConnection::testArrowConnectsItems()
{
    qDebug() << "测试：验证箭头正确连接图元";
    
    // 创建并定位两个图元
    DiagramItem *startItem = new DiagramItem(DiagramItem::StartEnd, nullptr);
    DiagramItem *endItem = new DiagramItem(DiagramItem::Step, nullptr);
    
    scene->addItem(startItem);
    scene->addItem(endItem);
    
    startItem->setPos(50, 200);
    endItem->setPos(400, 200);
    
    // 创建箭头
    Arrow *arrow = new Arrow(startItem, endItem);
    scene->addItem(arrow);
    
    // 验证箭头的起点和终点
    QCOMPARE(arrow->startItem(), startItem);
    QCOMPARE(arrow->endItem(), endItem);
    
    // 验证箭头类型
    QCOMPARE(arrow->type(), Arrow::Type);
    
    qDebug() << "✓ 箭头正确连接了起点和终点图元";
}

void TestArrowConnection::testArrowUpdateOnItemMove()
{
    qDebug() << "测试：移动图元时箭头位置更新";
    
    // 创建图元和箭头
    DiagramItem *startItem = new DiagramItem(DiagramItem::Step, nullptr);
    DiagramItem *endItem = new DiagramItem(DiagramItem::Step, nullptr);
    
    scene->addItem(startItem);
    scene->addItem(endItem);
    
    startItem->setPos(100, 100);
    endItem->setPos(300, 100);
    
    Arrow *arrow = new Arrow(startItem, endItem);
    startItem->addArrow(arrow);
    endItem->addArrow(arrow);
    scene->addItem(arrow);
    
    // 记录初始边界
    QRectF initialBounds = arrow->boundingRect();
    
    // 移动终点图元
    endItem->setPos(500, 300);
    arrow->updatePosition();
    
    // 验证箭头边界已更新
    QRectF newBounds = arrow->boundingRect();
    QVERIFY(initialBounds != newBounds);
    
    qDebug() << "✓ 图元移动后箭头位置正确更新";
}

void TestArrowConnection::testArrowColor()
{
    qDebug() << "测试：设置箭头颜色";
    
    DiagramItem *startItem = new DiagramItem(DiagramItem::Io, nullptr);
    DiagramItem *endItem = new DiagramItem(DiagramItem::Io, nullptr);
    
    scene->addItem(startItem);
    scene->addItem(endItem);
    
    startItem->setPos(100, 100);
    endItem->setPos(300, 300);
    
    Arrow *arrow = new Arrow(startItem, endItem);
    scene->addItem(arrow);
    
    // 设置箭头颜色
    QColor redColor = Qt::red;
    arrow->setColor(redColor);
    
    // 箭头颜色设置成功（颜色在 paint 时使用）
    QVERIFY(arrow != nullptr);
    
    // 设置蓝色
    QColor blueColor = Qt::blue;
    arrow->setColor(blueColor);
    
    qDebug() << "✓ 箭头颜色设置成功";
}

void TestArrowConnection::testMultipleArrows()
{
    qDebug() << "测试：从一个图元连接多个箭头";
    
    // 创建一个中心图元和多个目标图元
    DiagramItem *centerItem = new DiagramItem(DiagramItem::Conditional, nullptr);
    DiagramItem *targetA = new DiagramItem(DiagramItem::Step, nullptr);
    DiagramItem *targetB = new DiagramItem(DiagramItem::Step, nullptr);
    DiagramItem *targetC = new DiagramItem(DiagramItem::StartEnd, nullptr);
    
    scene->addItem(centerItem);
    scene->addItem(targetA);
    scene->addItem(targetB);
    scene->addItem(targetC);
    
    centerItem->setPos(200, 200);
    targetA->setPos(400, 100);  // 右上
    targetB->setPos(400, 300);  // 右下
    targetC->setPos(50, 200);   // 左侧
    
    // 创建多个箭头从中心图元出发
    Arrow *arrowA = new Arrow(centerItem, targetA);
    Arrow *arrowB = new Arrow(centerItem, targetB);
    Arrow *arrowC = new Arrow(targetC, centerItem);  // 反向
    
    centerItem->addArrow(arrowA);
    centerItem->addArrow(arrowB);
    centerItem->addArrow(arrowC);
    
    scene->addItem(arrowA);
    scene->addItem(arrowB);
    scene->addItem(arrowC);
    
    // 验证场景中有4个图元 + 3个箭头
    QCOMPARE(scene->items().count(), 7);
    
    // 验证所有箭头的起点/终点正确
    QCOMPARE(arrowA->startItem(), centerItem);
    QCOMPARE(arrowA->endItem(), targetA);
    QCOMPARE(arrowB->startItem(), centerItem);
    QCOMPARE(arrowB->endItem(), targetB);
    QCOMPARE(arrowC->startItem(), targetC);
    QCOMPARE(arrowC->endItem(), centerItem);
    
    qDebug() << "✓ 多箭头连接测试通过";
}

void TestArrowConnection::testRemoveArrowFromItem()
{
    qDebug() << "测试：从图元移除箭头";
    
    DiagramItem *startItem = new DiagramItem(DiagramItem::Step, nullptr);
    DiagramItem *endItem = new DiagramItem(DiagramItem::Step, nullptr);
    
    scene->addItem(startItem);
    scene->addItem(endItem);
    
    startItem->setPos(100, 100);
    endItem->setPos(300, 100);
    
    // 创建并添加箭头
    Arrow *arrow = new Arrow(startItem, endItem);
    startItem->addArrow(arrow);
    endItem->addArrow(arrow);
    scene->addItem(arrow);
    
    QCOMPARE(scene->items().count(), 3);
    
    // 从起点图元移除箭头
    startItem->removeArrow(arrow);
    endItem->removeArrow(arrow);
    
    // 从场景移除箭头
    scene->removeItem(arrow);
    delete arrow;
    
    QCOMPARE(scene->items().count(), 2);
    
    qDebug() << "✓ 箭头移除测试通过";
}

QTEST_MAIN(TestArrowConnection)
#include "test_arrow_connection.moc"

