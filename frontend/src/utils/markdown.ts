/**
 * Markdown 渲染工具
 */
import { createMarkdownExit } from 'markdown-exit'

// 创建 markdown 实例
const md = createMarkdownExit({
  html: true,        // 允许 HTML 标签
  linkify: true,     // 自动转换 URL 为链接
  typographer: true, // 启用智能引号和其他排版替换
  breaks: true,      // 转换换行符为 <br>
})

/**
 * 渲染 Markdown 为 HTML
 */
export function renderMarkdown(content: string): string {
  try {
    return md.render(content)
  } catch (error) {
    console.error('Markdown 渲染失败:', error)
    return content
  }
}

/**
 * 渲染 Markdown 为纯文本（移除 HTML 标签）
 */
export function renderMarkdownText(content: string): string {
  const html = renderMarkdown(content)
  const div = document.createElement('div')
  div.innerHTML = html
  return div.textContent || div.innerText || ''
}
