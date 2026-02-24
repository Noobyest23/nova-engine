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

const navHTML = `
<div class="logo">
    <img src="Images/engine-icon.png" alt="Nova Engine Logo">
</div>
<h2>Navigation</h2>
<ul>
    <li><a href="index.html">Home</a></li>
    <li><a href="scene-structure.html">Scene File Structure</a></li>
    <li class="nav-dropdown">
        <details>
            <summary>Assets</summary>
            <ul>
                <li><a href="asset.html">Asset</a></li>
				<li class="nav-dropdown">
					<details>
						<summary>Images</summary>
						<ul>
							<li><a href="image.html">Image</a></li>
							<li><a href="file-image.html">FileImage</a></li>
						</ul>
					</details>
				</li>
				<li class="nav-dropdown">
					<details>
						<summary>Mesh2D</summary>
						<ul>
							<li><a href="mesh2d.html">Mesh2D</a></li>
							<li><a href="boxmesh2d.html">BoxMesh2D</a></li>
							<li><a href="circlemesh2d.html">CircleMesh2D</a></li>
						</ul>
					</details>
				</li>
                <li><a href="script.html">Script</a></li>
				<li><a href="material.html">Material</a></li>
            </ul>
        </details>
    </li>
</ul>`;

document.addEventListener("DOMContentLoaded", () => {
	const navBar = document.querySelector('nav');
	if (navBar) {
		navBar.innerHTML = navHTML;
	}
});