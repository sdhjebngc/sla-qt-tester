"""
数据库模块
管理测试历史记录和截图
"""
from .db_manager import TestDatabase
from .models import TestRun, TestCaseDetail, Screenshot

__all__ = ['TestDatabase', 'TestRun', 'TestCaseDetail', 'Screenshot']
