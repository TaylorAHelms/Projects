%function to update centroidsfunction centroids = updateCentroids(X, indices, K)    centroids = zeros(K, size(X,2));    for i=1:K        xi = X(indices==i,:);        centroids(i,:) = [sum(xi(:,1)) sum(xi(:,2))]/(size(xi,1));    endend