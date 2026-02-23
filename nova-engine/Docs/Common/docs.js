// Nova Engine Docs Shared JS
// Handles navigation, highlighting, and any shared logic

function showSection(hash) {
	const main = document.querySelector('main');
	if (!main) return;
	main.querySelectorAll('section').forEach(sec => {
		sec.classList.remove('active');
	});
	const target = main.querySelector(hash);
	if (target) target.classList.add('active');
}

// Highlight code blocks (for highlight.js)
document.addEventListener('DOMContentLoaded', () => {
	if (window.hljs) hljs.highlightAll();
});

// If using hash navigation, highlight after section change
window.addEventListener('hashchange', () => {
	if (window.hljs) hljs.highlightAll();
});

fetch('nav.html')
        .then(response => response.text())
        .then(data => {
            document.getElementById('side-nav').innerHTML = data;
        });
